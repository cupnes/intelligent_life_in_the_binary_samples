#include <protein.h>
#include <cell.h>
#include <bio_sys.h>

#define MAX_POOL_CODONS	100
#define MAX_POOL_CELLS	100
#define BOND_BUF_SIZE	100

static struct cell cell_pool[MAX_POOL_CELLS];
static unsigned int is_cell_creation;
static struct codon codn_pool[MAX_POOL_CODONS];
static unsigned int is_codn_creation;
static unsigned char bond_buf[BOND_BUF_SIZE];

static void init_cell(struct cell *cell)
{
	/* ヘッダ */
	cell->list.next = NULL;

	/* タンパク質 */
	cell->prot_head.next = NULL;

	/* DNA */
	cell->codn_head.next = NULL;

	/* 属性情報 */
	cell->life_duration = DEFAULT_LIFE_DURATION;
	cell->life_left = DEFAULT_LIFE_DURATION;
}

static void metabolism_and_motion(struct cell *cell)
{
	/* データ化合物を一つ取得 */
	struct compound *comp = biosys_pop_compound(COMP_FILTER_DATA);
	if (comp == NULL)
		return;

	/* 全てのタンパク質とその中の化合物が持つデータをbond_buf内で結合 */
	unsigned int idx = 0;
	struct singly_list *prot;
	for (prot = cell->prot_head.next; prot != NULL; prot = prot->next)
		idx += protein_bond_compounds(
			(struct protein *)prot, &bond_buf[idx]);

	/* bond_buf[]を関数として実行 */
	bio_data_t (*func)(bio_data_t);
	func = (void *)bond_buf;
	bio_data_t prod_val = (*func)(comp->elements.data);

	/* 戻り値で元の化合物を更新 */
	comp->elements.data = prod_val;

	/* 化合物を環境へ排出する */
	biosys_push_compound(comp);
}

static bool_t growth(struct cell *cell)
{
	/* コード化合物を一つ取得 */
	struct compound *comp = biosys_pop_compound(COMP_FILTER_CODE);

	struct singly_list *entry;

	/* 空きのあるコドンがあれば結合する */
	for (entry = cell->codn_head.next; entry != NULL; entry = entry->next) {
		struct codon *codn = (struct codon *)entry;
		if ((codn->comp_data == comp->elements.data)
		    && (codn->buf == NULL)) {
			codn->buf = comp;
			comp = NULL;
			break;
		}
	}

	/* 結合しなかった場合、環境へ排出する */
	if (comp != NULL)
		biosys_push_compound(comp);

	/* 細胞分裂可能化否かを判定 */
	for (entry = cell->codn_head.next; entry != NULL; entry = entry->next) {
		struct codon *codn = (struct codon *)entry;
		if (codn->buf == NULL)
			return FALSE;
	}
	return TRUE;
}

static struct singly_list *central_dogma(struct cell *cell)
{
	struct singly_list *codn_ptr = cell->codn_head.next;

	struct singly_list *e1, *e2;

	struct singly_list prot_list;
	struct singly_list *parent_prot = &prot_list;
	struct protein *prot_new;
	for (e1 = cell->prot_head.next; e1 != NULL; e1 = e1->next) {
		prot_new = protein_create();
		if (prot_new == NULL)
			die("central_dogma: can't create protein.");

		struct singly_list *parent_comp = &prot_new->comp_head;
		struct protein *prot = (struct protein *)e1;
		struct compound *comp_codn;
		for (e2 = prot->comp_head.next; e2 != NULL; e2 = e2->next) {
			/* struct compound *comp = (struct compound *)e2; */
			struct codon *codn = (struct codon *)codn_ptr;
			if (codn == NULL)
				die("central_dogma: DNA error(codn length)");
			comp_codn = codn->buf;

			/* ウィルス感染デモ用に無効化 */
			/* if (comp->elements.data != comp_codn->elements.data) */
			/* 	die("central_dogma: DNA error(prot != codn)"); */

			parent_comp->next = &comp_codn->list;
			codn->buf = NULL;
			codn_ptr = codn_ptr->next;

			parent_comp = &comp_codn->list;
		}
		comp_codn->list.next = NULL;

		parent_prot->next = &prot_new->list;

		parent_prot = &prot_new->list;
	}
	prot_new->list.next = NULL;

	return prot_list.next;
}

static struct singly_list *copy_codon_list(struct singly_list *codn_head)
{
	struct singly_list *new_entry_1st = NULL;
	struct codon *new_codon = NULL;
	struct singly_list *new_entry_prev = NULL;

	struct singly_list *orig_entry;
	struct codon *orig_codon;

	for (orig_entry = codn_head->next; orig_entry != NULL;
	     orig_entry = orig_entry->next) {
		new_codon = codon_create();
		if (new_codon == NULL)
			die("copy_codon_list: can't create codon.");

		orig_codon = (struct codon *)orig_entry;
		new_codon->comp_data = orig_codon->comp_data;

		if (new_entry_1st == NULL)
			new_entry_1st = &new_codon->list;
		else
			new_entry_prev->next = &new_codon->list;

		new_entry_prev = &new_codon->list;
	}

	return new_entry_1st;
}

static void division(struct cell *cell)
{
	/* 細胞の構造を生成 */
	struct cell *cell_new = cell_create();
	if (cell_new == NULL)
		return;

	/* 現細胞のDNAから生成したタンパク質リストを新細胞へ繋ぐ */
	cell_new->prot_head.next = central_dogma(cell);

	/* DNAを複製 */
	cell_new->codn_head.next = copy_codon_list(&cell->codn_head);

	/* 属性情報を遺伝 */
	cell_new->life_duration = cell->life_duration;
	cell_new->life_left = cell_new->life_duration;

	/* 新細胞を環境へ放出 */
	biosys_push_cell(cell_new);
}

static void death(struct cell *cell)
{
	/* 細胞の構成要素をバラバラにして環境へ放出 */
	struct singly_list *e1, *n1;
	struct singly_list *e2, *n2;

	/* タンパク質 */
	for (e1 = cell->prot_head.next; e1 != NULL; e1 = n1) {
		n1 = e1->next;
		struct protein *prot = (struct protein *)e1;
		for (e2 = prot->comp_head.next; e2 != NULL; e2 = n2) {
			n2 = e2->next;
			e2->next = NULL;
			biosys_push_compound((struct compound *)e2);
		}
		prot->comp_head.next = NULL;
		prot->list.next = NULL;
		prot->in_use = FALSE;
	}
	cell->prot_head.next = NULL;

	/* DNA */
	for (e1 = cell->codn_head.next; e1 != NULL; e1 = n1) {
		n1 = e1->next;
		struct codon *codn = (struct codon *)e1;
		if (codn->buf != NULL) {
			struct compound *comp = (struct compound *)codn->buf;
			comp->list.next = NULL;
			biosys_push_compound(comp);
			codn->buf = NULL;
		}
		codn->comp_data = 0;
		codn->list.next = NULL;
		codn->in_use = FALSE;
	}
	cell->codn_head.next = NULL;


	/* 環境から細胞を削除 */
	biosys_remove_cell(cell);
	cell->life_duration = cell->life_left = 0;
	cell->list.next = NULL;
	cell->in_use = FALSE;
}

void cell_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++)
		codn_pool[i].in_use = FALSE;
	for (i = 0; i < MAX_POOL_CELLS; i++)
		cell_pool[i].in_use = FALSE;
}

struct cell *cell_create(void)
{
	spin_lock(&is_cell_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CELLS; i++) {
		if (cell_pool[i].in_use == FALSE) {
			cell_pool[i].in_use = TRUE;
			spin_unlock(&is_cell_creation);
			init_cell(&cell_pool[i]);
			return &cell_pool[i];
		}
	}

	spin_unlock(&is_cell_creation);
	return NULL;
}

struct codon *codon_create(void)
{
	spin_lock(&is_codn_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_CODONS; i++) {
		if (codn_pool[i].in_use == FALSE) {
			codn_pool[i].in_use = TRUE;
			spin_unlock(&is_codn_creation);
			codn_pool[i].list.next = NULL;
			codn_pool[i].comp_data = 0;
			codn_pool[i].buf = NULL;
			return &codn_pool[i];
		}
	}

	spin_unlock(&is_codn_creation);
	return NULL;
}

struct codon *codon_create_with_data(bio_data_t data)
{
	struct codon *codn = codon_create();
	if (codn == NULL)
		return NULL;

	codn->comp_data = data;
	return codn;
}

bool_t cell_run(struct cell *cell)
{
	/* 代謝/運動 */
	metabolism_and_motion(cell);

	/* 成長 */
	bool_t is_divisible = growth(cell);
	if (is_divisible == TRUE) {
		/* 増殖 */
		division(cell);
	}

	/* 寿命を減らす */
	bool_t is_dead = FALSE;
	cell->life_left--;
	if (cell->life_left == 0) {
		/* 死 */
		death(cell);
		is_dead = TRUE;
	}

	return is_dead;
}

void cell_dump_entry(struct cell *cell)
{
	putchar('{');
	struct singly_list *entry;
	for (entry = cell->prot_head.next; entry != NULL; entry = entry->next) {
		protein_dump_entry((struct protein *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
	putchar('}');

	putchar('(');
	putd(cell->life_left, 2);
	putchar(')');
}

void cell_dump_list(struct singly_list *list_head)
{
	struct singly_list *entry;
	for (entry = list_head->next; entry != NULL; entry = entry->next) {
		cell_dump_entry((struct cell *)entry);
		if (entry->next != NULL) {
			/* putchar(','); */
			puts("\r\n");
		}
	}
}
