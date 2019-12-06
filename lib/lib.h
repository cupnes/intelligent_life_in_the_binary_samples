#ifndef _LIB_H_
#define _LIB_H_

#ifdef RUN_LOCAL
#include <stdio.h>
#include <string.h>
#endif

#ifndef RUN_LOCAL
#define NULL	(void *)0
#endif

#define TRUE	1
#define FALSE	0

#define MAX_FILES	1000

#ifndef RUN_LOCAL
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#else
#define SCREEN_WIDTH	100
#define SCREEN_HEIGHT	35
#endif

#define FONT_WIDTH	24
#define FONT_HEIGHT	32

#define KEY_ENTER	0x0a
#define KEY_ESC		0x1b
#define KEY_DOWN	0x32
#define KEY_LEFT	0x34
#define KEY_RIGHT	0x36
#define KEY_UP		0x38

#define MIN(A, B)	(((A) <= (B)) ? (A) : (B))

#define CPU_PAUSE()	asm volatile ("pause")

typedef unsigned char bool_t;

#ifndef RUN_LOCAL
typedef unsigned long long size_t;
#endif

enum SYSCCALL_NO {
	SYSCALL_PUTC,
	SYSCALL_GETC,
	SYSCALL_VPUTC,
	SYSCALL_FG,
	SYSCALL_BG,
	SYSCALL_CLS,
	SYSCALL_VCUR_RST,
	SYSCALL_KBC_HDLR,
	SYSCALL_OPEN,
	SYSCALL_GET_FILES,
	SYSCALL_EXEC,
	SYSCALL_ENQ_TASK,
	SYSCALL_RCV_PKT,
	SYSCALL_SND_PKT,
	SYSCALL_MOV_CUR,
	SYSCALL_GET_CUR_Y,
	SYSCALL_DRAW_BG,
	SYSCALL_DRAW_FG,
	SYSCALL_DRAW_IMG,
	SYSCALL_GET_DATETIME,
	SYSCALL_SLEEP,
	SYSCALL_GET_PX,
	SYSCALL_FINISH_TASK,
	SYSCALL_GET_MAC,
	SYSCALL_DRAW_PX_FG,
	SYSCALL_DRAW_PX_BG,
	SYSCALL_NIC_RX_EN,
	SYSCALL_SER_GETC,
	SYSCALL_SER_PUTC,
	SYSCALL_IS_ALIVE,
	SYSCALL_FINISH_CURRENT_TASK,
	SYSCALL_EXEC_AP,
	SYSCALL_FILL_RECT,
	MAX_SYSCALL_NUM
};

enum FONT_IDX {
	FONT_ascii_20_spc = 0x20,
	FONT_ascii_21_exc,
	FONT_ascii_22_dquote,
	FONT_ascii_23_sharp,
	FONT_ascii_24_dollar,
	FONT_ascii_25_percent,
	FONT_ascii_26_and,
	FONT_ascii_27_squote,
	FONT_ascii_28_rbracket_open,
	FONT_ascii_29_rbracket_close,
	FONT_ascii_2A_asterisk,
	FONT_ascii_2B_plus,
	FONT_ascii_2C_comma,
	FONT_ascii_2D_hyphen,
	FONT_ascii_2E_dot,
	FONT_ascii_2F_slash,
	FONT_ascii_30_0,
	FONT_ascii_31_1,
	FONT_ascii_32_2,
	FONT_ascii_33_3,
	FONT_ascii_34_4,
	FONT_ascii_35_5,
	FONT_ascii_36_6,
	FONT_ascii_37_7,
	FONT_ascii_38_8,
	FONT_ascii_39_9,
	FONT_ascii_3A_colon,
	FONT_ascii_3B_semicolon,
	FONT_ascii_3C_lt,
	FONT_ascii_3D_equal,
	FONT_ascii_3E_gt,
	FONT_ascii_3F_question,
	FONT_ascii_40_atmark,
	FONT_ascii_41_A,
	FONT_ascii_42_B,
	FONT_ascii_43_C,
	FONT_ascii_44_D,
	FONT_ascii_45_E,
	FONT_ascii_46_F,
	FONT_ascii_47_G,
	FONT_ascii_48_H,
	FONT_ascii_49_I,
	FONT_ascii_4A_J,
	FONT_ascii_4B_K,
	FONT_ascii_4C_L,
	FONT_ascii_4D_M,
	FONT_ascii_4E_N,
	FONT_ascii_4F_O,
	FONT_ascii_50_P,
	FONT_ascii_51_Q,
	FONT_ascii_52_R,
	FONT_ascii_53_S,
	FONT_ascii_54_T,
	FONT_ascii_55_U,
	FONT_ascii_56_V,
	FONT_ascii_57_W,
	FONT_ascii_58_X,
	FONT_ascii_59_Y,
	FONT_ascii_5A_Z,
	FONT_ascii_5B_sbracket_open,
	FONT_ascii_5C_yen,
	FONT_ascii_5D_sbracket_close,
	FONT_ascii_5E_hat,
	FONT_ascii_5F_underscore,
	FONT_ascii_60_bquote,
	FONT_ascii_61_a,
	FONT_ascii_62_b,
	FONT_ascii_63_c,
	FONT_ascii_64_d,
	FONT_ascii_65_e,
	FONT_ascii_66_f,
	FONT_ascii_67_g,
	FONT_ascii_68_h,
	FONT_ascii_69_i,
	FONT_ascii_6A_j,
	FONT_ascii_6B_k,
	FONT_ascii_6C_l,
	FONT_ascii_6D_m,
	FONT_ascii_6E_n,
	FONT_ascii_6F_o,
	FONT_ascii_70_p,
	FONT_ascii_71_q,
	FONT_ascii_72_r,
	FONT_ascii_73_s,
	FONT_ascii_74_t,
	FONT_ascii_75_u,
	FONT_ascii_76_v,
	FONT_ascii_77_w,
	FONT_ascii_78_x,
	FONT_ascii_79_y,
	FONT_ascii_7A_z,
	FONT_ascii_7B_cbracket_open,
	FONT_ascii_7C_bar,
	FONT_ascii_7D_cbracket_close,
	FONT_ascii_7E_tilde,
	FONT_hira_a = 0x80,
	FONT_hira_chi,
	FONT_hira_de,
	FONT_hira_e,
	FONT_hira_fu,
	FONT_hira_ha,
	FONT_hira_he,
	FONT_hira_hi,
	FONT_hira_ho,
	FONT_hira_i,
	FONT_hira_ka,
	FONT_hira_ke,
	FONT_hira_ki,
	FONT_hira_ko,
	FONT_hira_ku,
	FONT_hira_ma,
	FONT_hira_me,
	FONT_hira_mi,
	FONT_hira_mo,
	FONT_hira_mu,
	FONT_hira_n,
	FONT_hira_na,
	FONT_hira_ne,
	FONT_hira_ni,
	FONT_hira_no,
	FONT_hira_nu,
	FONT_hira_o,
	FONT_hira_pe,
	FONT_hira_ra,
	FONT_hira_re,
	FONT_hira_ri,
	FONT_hira_ro,
	FONT_hira_ru,
	FONT_hira_sa,
	FONT_hira_se,
	FONT_hira_shi,
	FONT_hira_so,
	FONT_hira_su,
	FONT_hira_ta,
	FONT_hira_te,
	FONT_hira_to,
	FONT_hira_tsu,
	FONT_hira_u,
	FONT_hira_wa,
	FONT_hira_wo,
	FONT_hira_xya,
	FONT_hira_ya,
	FONT_hira_yo,
	FONT_hira_yu,
	FONT_kata_chi,
	FONT_kata_fu,
	FONT_kata_ji,
	FONT_kata_ke,
	FONT_kata_ku,
	FONT_kata_pa,
	FONT_kata_ra,
	FONT_kata_ru,
	FONT_kata_su,
	FONT_kata_to,
	FONT_kata_xtu,
	FONT_kata_xyu,
	FONT_x_cho,
	FONT_x_dai,
	FONT_x_han,
	FONT_x_hatsu,
	FONT_x_hon,
	FONT_x_ji,
	FONT_x_kami,
	FONT_x_saku,
	FONT_x_shin,
	FONT_x_shu,
	FONT_x_toku,
	FONT_x_yu,
	FONT_x_yuki,
	FONT_x_dakuten,
	FONT_x_handakuten,
	FONT_MAX
};

#define FILE_NAME_LEN	28
struct __attribute__((packed)) file {
	char name[FILE_NAME_LEN];
	unsigned int size;
	unsigned char data[0];
};

struct textfile {
	unsigned int idx;
	struct file *file;
};

struct __attribute__((packed)) pixelformat {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char _reserved;
};

struct __attribute__((packed)) framebuffer {
	struct pixelformat *base;
	unsigned long long size;
	unsigned int hr;
	unsigned int vr;
};

struct __attribute__((packed)) datetime {
	unsigned short year;
	unsigned char mon;
	unsigned char day;
	unsigned char week;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
};

struct __attribute__((packed)) image {
	unsigned int width;
	unsigned int height;
	struct pixelformat data[0];
};

struct __attribute__((packed)) rect {
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
};

#ifndef RUN_LOCAL
int strcmp(char *s1, char *s2);
int strncmp(char *s1, char *s2, unsigned long long n);
unsigned long long strnidx(char s[], char c, unsigned long long size);
void memcpy(void *dst, void *src, unsigned long long size);
#endif
unsigned long long syscall(
	unsigned long long syscall_id __attribute__((unused)),
	unsigned long long arg1 __attribute__((unused)),
	unsigned long long arg2 __attribute__((unused)),
	unsigned long long arg3 __attribute__((unused)));
#ifndef RUN_LOCAL
void putchar(char c);
void puts(char *s);
#endif
void putd(unsigned long long val, unsigned char num_digits);
void puth(unsigned long long val, unsigned char num_digits);
void die(char *msg);
#ifndef RUN_LOCAL
char getchar(void);
#endif
void vputc(unsigned char c);
void vputs(unsigned char *s);
void vputh(unsigned long long val, unsigned char num_digits);
void set_fg(unsigned char r, unsigned char g, unsigned char b);
void set_bg(unsigned char r, unsigned char g, unsigned char b);
void clear_screen(void);
void vcursor_reset(void);
void set_kbc_handler(void *handler);
struct file *open(char *file_name);
unsigned long long get_files(struct file *files[]);
unsigned long long get_files_with_prefix(
	struct file *files[], char prefix);
void exec(struct file *file);
int exec_bg(struct file *file);
void exec_ap(struct file *file, unsigned char pnum);
unsigned short receive_packet(void *p_data);
void send_packet(void *p_data, unsigned short p_len);
void move_cursor(unsigned int x, unsigned int y);
void move_cursor_text(unsigned int x, unsigned int y);
unsigned int get_cursor_y(void);
void get_px(unsigned int x, unsigned int y, struct pixelformat *val);
void draw_px_fg(unsigned int x, unsigned int y);
void draw_px_bg(unsigned int x, unsigned int y);
void draw_bg(struct file *img);
void draw_fg(struct file *img);
void draw_image(struct image *img, unsigned int px, unsigned int py);
void fill_rect(struct rect *rect, struct pixelformat *color);
void image_viewer(struct image *img);
void anime_viewer(struct file *img_list, unsigned int px, unsigned int py,
		  unsigned long long interval_us);
void make_mask_region(unsigned int base_x, unsigned int base_y,
		      unsigned int width, unsigned int height,
		      struct image *mask);
void get_datetime(struct datetime *dt);
void sleep(unsigned long long us);
void finish_task(int task_id);
void finish_current_task(void);
void get_mac(unsigned char *mac);
void nic_rx_enable(void);
#ifndef RUN_LOCAL
void set_seed(unsigned short seed);
unsigned short rand(void);
#endif
char ser_getc(void);
void ser_putc(char c);
char *file_read_line(char buf[], unsigned int buf_len, struct textfile *text);
unsigned char is_alive(int task_id);

extern struct framebuffer *fb;

/* Spin Lock */
void spin_lock(unsigned int *lock_flag);
void spin_unlock(volatile unsigned int *lock_flag);

/* Singly List (Linear) */
struct singly_list {
	struct singly_list *next;
};
void slist_prepend(struct singly_list *entry, struct singly_list *head);
void slist_append(struct singly_list *entry, struct singly_list *head);
struct singly_list *slist_remove(
	struct singly_list *entry, struct singly_list *head);
struct singly_list *slist_find_in(
	struct singly_list *target, struct singly_list *list);

#endif
