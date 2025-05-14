#include <math.h>

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static;
#define internal static;

#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) !input->buttons[b].is_down && input->buttons[b].changed

#define process_button_win32(b, vk) case (vk): {input.buttons[b].changed = is_down != input.buttons[b].is_down; input.buttons[b].is_down = is_down;} break;

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

inline float
distance_px(int x0, int y0, int x1, int y1) {
	int dist_x = x1 - x0;
	int dist_y = y1 - y0;
	return sqrt(dist_x + dist_y);
}

inline bool
coll_aabb(float p1x, float p1y, float hs1x, float hs1y, float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1x > p2y - hs2y &&
		p1y - hs1x < p2y + hs2y);
}

