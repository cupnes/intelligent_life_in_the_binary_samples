#include <protein.h>
#include <cell.h>
#include <lives/incrementer.h>

#define MAX_BIO_FILES	100
#define MAX_CYCLES	100
#define SLEEP_CYCLE_US	1000000	/* 1,000,000us (1s) */

#define BG_R	0
#define BG_G	0
#define BG_B	0

#define FG_R	255
#define FG_G	255
#define FG_B	255

#define PUT_INCR_COMP_CYCLE	10
#define VIRUS_INFECTION_CYCLE	30

#define INITIAL_DATA	0

struct bio_env {
	struct singly_list cell_head;
	struct singly_list comp_head;
};

static struct bio_env be;

static void init_bio_env(void);
static void dump_bio_status(unsigned int cycle_num);
static void run_bio_cycle(void);
static void run_bio_cycle_hook(unsigned int cycle_num);

int main(void)
{
	/* 生体環境を初期化 */
	init_bio_env();

	/* 背景色・描画色を設定 */
	set_bg(BG_R, BG_G, BG_B);
	set_fg(FG_R, FG_G, FG_B);

	/* 0周期目 */
	unsigned int cycle_num = 0;
	clear_screen();			/* 画面クリア */
	dump_bio_status(cycle_num);	/* 現在の状態をダンプ */
	sleep(SLEEP_CYCLE_US);		/* 次の周期を待つ */

	for (cycle_num = 1; cycle_num < MAX_CYCLES; cycle_num++) {
		/* 画面クリア */
		clear_screen();

		/* 生体環境の1周期を実施 */
		run_bio_cycle();

		/* 周期毎追加処理を呼び出す */
		run_bio_cycle_hook(cycle_num);

		/* 現在の状態をダンプ */
		dump_bio_status(cycle_num);

		/* 次の周期を待つ */
		sleep(SLEEP_CYCLE_US);
	}

	return 0;
}

static void init_bio_env(void)
{
	/* プール領域の初期化 */
	compound_pool_init();
	protein_pool_init();
	cell_pool_init();

	struct singly_list *entry;

	/* 環境に配置する生物を生成しリスト化 */
	entry = (struct singly_list *)incrementer_create();
	be.cell_head.next = entry;
	entry->next = NULL;

	/* 環境に配置する化合物を生成しリスト化 */
	entry = (struct singly_list *)compound_create_with_data(INITIAL_DATA);
	be.comp_head.next = entry;
	entry->next = NULL;
}

static void dump_bio_status(unsigned int cycle_num)
{
	puts("    ### ");
	putd(cycle_num, 2);
	puts("th cycle ###\r\n");

	puts("\r\n");

	puts("Cells:\r\n");
	cell_dump_list(&be.cell_head);
	puts("\r\n");
	puts("\r\n");

	puts("Data Compounds:\r\n");
	compound_dump_list(&be.comp_head, COMP_FILTER_DATA);
	puts("\r\n");
	puts("\r\n");

	puts("Code Compounds:\r\n");
	compound_dump_list(&be.comp_head, COMP_FILTER_CODE);
}

static void run_bio_cycle(void)
{
	/* 生体実行ファイルを全て実行 */
	struct singly_list *entry;
	for (entry = be.cell_head.next; entry != NULL; entry = entry->next)
		cell_run((struct cell *)entry);
}

static void infect_virus(struct cell *cell)
{
	struct singly_list *mov = cell->prot_head.next;
	struct singly_list *inc = mov->next;

	struct protein *prot = (struct protein *)inc;

	struct singly_list *rex = prot->comp_head.next;
	struct singly_list *opcode = rex->next;
	struct singly_list *operand = opcode->next;

	struct compound *comp = (struct compound *)operand;

	if (comp->elements.bytes[0] != 0xc0)
		die("infect_virus: this is not inc instruction.");

	comp->elements.bytes[0] = 0xc8;
}

static void run_bio_cycle_hook(unsigned int cycle_num)
{
	struct singly_list *comp_1st_entry;

	switch (cycle_num) {
	case PUT_INCR_COMP_CYCLE:
		comp_1st_entry = incrementer_create_essential_compounds();
		slist_append(comp_1st_entry, &be.comp_head);
		break;

	case VIRUS_INFECTION_CYCLE:
		infect_virus((struct cell *)be.cell_head.next);
		break;
	}
}

struct compound *biosys_pop_compound(enum comp_filter filter)
{
	if (be.comp_head.next == NULL)
		return NULL;

	struct singly_list *entry;

	if (filter == COMP_FILTER_NONE) {
		entry = be.comp_head.next;
		be.comp_head.next = entry->next;
		return (struct compound *)entry;
	}

	struct singly_list *prev = &be.comp_head;
	for (entry = be.comp_head.next; entry != NULL; entry = entry->next) {
		struct compound *comp = (struct compound *)entry;

		if (((filter == COMP_FILTER_CODE)
		     && (compound_is_data(comp) == FALSE))
		    || ((filter == COMP_FILTER_DATA)
			&& (compound_is_data(comp) == TRUE))) {
			prev->next = entry->next;
			entry->next = NULL;
			return comp;
		}

		prev = entry;
	}

	return NULL;
}

void biosys_push_compound(struct compound *comp)
{
	slist_append(&comp->list, &be.comp_head);
}

void biosys_push_cell(struct cell *cell)
{
	slist_prepend(&cell->list, &be.cell_head);
}

void biosys_remove_cell(struct cell *cell)
{
	struct singly_list *removed = slist_remove(&cell->list, &be.cell_head);
	if ((removed == NULL) || (removed != &cell->list))
		die("biosys_remove_cell: target cell not found.");
}
