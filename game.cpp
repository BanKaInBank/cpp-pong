bool use_bot = true;

float player1_p, player1_dp, player2_p, player2_dp;
float player_h_size_x = 2.5, player_h_size_y = 12;

float arena_h_size_x = 85, arena_h_size_y = 45;

float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y;
float ball_h_size = 1.f;

u32 player1_score, player2_score;

internal void
update_player(float *p, float *dp, float ddp, float *delta) {
	ddp -= *dp * 10.f; // Friction

	*p += *dp * *delta + ddp * *delta * *delta * .5f; // Position
	*dp = *dp + ddp * *delta; // Velocity

	// Second player collision
	if (*p + player_h_size_y > arena_h_size_y) {
		*p = arena_h_size_y - player_h_size_y;
		*dp = 0;
	}
	else if (*p - player_h_size_y < -arena_h_size_y) {
		*p = -arena_h_size_y + player_h_size_y;
		*dp = 0;
	}
}

internal void
update_game(Input* input, float delta) {
	// First player controls	
	float player1_ddp = 0.f;
	if (is_down(BUTTON_UP)) player1_ddp += 2000;
	if (is_down(BUTTON_DOWN)) player1_ddp -= 2000;
	

	update_player(&player1_p, &player1_dp, player1_ddp, &delta);

	// Second player controls
	float player2_ddp = 0.f;
	if (!use_bot) {
		if (is_down(BUTTON_W)) player2_ddp += 2000;
		if (is_down(BUTTON_S)) player2_ddp -= 2000;
	}
	else {
		if (ball_p_y > player2_p && ball_p_x < arena_h_size_x / 2) player2_ddp += 1500;
		if (ball_p_y < player2_p && ball_p_x < arena_h_size_x / 2) player2_ddp -= 1500;
	}
	
	update_player(&player2_p, &player2_dp, player2_ddp, &delta);
	

	// Handle ball
	{
		ball_p_x += ball_dp_x * delta;
		ball_p_y += ball_dp_y * delta;

		// Ball collision (Player 1 & Player 2)
		if (coll_aabb(ball_p_x, ball_p_y, ball_h_size, ball_h_size, 80, player1_p, player_h_size_x, player_h_size_y)) {
			ball_p_x = 80 - player_h_size_x - ball_h_size;
			ball_dp_x *= -1;
			ball_dp_y = player1_dp;
			
		}
		else if (coll_aabb(ball_p_x, ball_p_y, ball_h_size, ball_h_size, -80, player2_p, player_h_size_x, player_h_size_y)) {
			ball_p_x = -80 + player_h_size_x + ball_h_size;
			ball_dp_x *= -1;
			ball_dp_y = player2_dp;
		}
		// Ball collision (Arena Up/Down/Left/Right)
		if (ball_p_y + ball_h_size > arena_h_size_y) {
			ball_p_y = arena_h_size_y - ball_h_size;
			ball_dp_y *= -1;
		}
		else if (ball_p_y - ball_h_size < -arena_h_size_y) {
			ball_p_y = -arena_h_size_y + ball_h_size;
			ball_dp_y *= -1;
		}
		else if (ball_p_x - ball_h_size < -arena_h_size_x) {
			ball_p_x = 0;
			ball_p_y = 0;
			ball_dp_x = 100;
			ball_dp_y = 0;
			player1_score++;
		}
		else if (ball_p_x + ball_h_size > arena_h_size_x) {
			ball_p_x = 0;
			ball_p_y = 0;
			ball_dp_x = 100;
			ball_dp_y = 0;
			player2_score++;
		}
	}

	// Rendering
	clear_screen(0xff5500);
	draw_rect(0, 0, arena_h_size_x, arena_h_size_y, 0xffaa33);

	draw_rect(ball_p_x, ball_p_y, ball_h_size, ball_h_size, 0xffffff);

	draw_rect(80, player1_p, player_h_size_x, player_h_size_y, 0xff0000); // Draw player 1
	draw_rect(-80, player2_p, player_h_size_x, player_h_size_y, 0xff0000); // Draw player 2

	draw_number(player1_score, -10, 40, 1.f, 0xbbffbb);
	draw_number(player2_score, 10, 40, 1.f, 0xbbffbb);
}