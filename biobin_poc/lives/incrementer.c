#include <protein.h>
#include <cell.h>

#define NUM_COMPOUNDS	7
#define INCREMENTER_LIFE_DURATION	40

/* インクリメンタ細胞の機械語コード
protein0: REX Prefix=0x48, Opcode=0x89, Operand(ModR/M)=0xf8	mov %rdi,%rax
protein1: REX Prefix=0x48, Opcode=0xff, Operand(ModR/M)=0xc0	inc %rax
protein2:                  Opcode=0xc3				ret
 */

static void create_comp_codn_with_elem(
	struct compound **comp, struct codon **codn,
	element_t *elem, unsigned int len)
{
	*comp = compound_create_with_elements(elem, len);
	if (*comp == NULL)
		die("create_comp_codn_with_elem: can't create a compound.");
	*codn = codon_create_with_data((*comp)->elements.data);
	if (*codn == NULL)
		die("create_comp_codn_with_elem: can't create a codon.");
}

struct cell *incrementer_create(void)
{
	/* 空の細胞を生成 */
	struct cell *cell = cell_create();
	if (cell == NULL)
		die("incrementer_create: can't create cell.");

	/* タンパク質のリスト(関数)を生成し細胞へ配置 */
	element_t elem[3];
	struct compound *comp[3];
	struct codon *codn[3];
	struct singly_list codn_head, codn_tail;
	struct protein *prot[3];

	unsigned int i;

	/* protein0: REX Prefix=0x48, Opcode=0x89, Operand(ModR/M)=0xf8 */
	elem[0] = 0x48; elem[1] = 0x89; elem[2] = 0xf8;
	for (i = 0; i < 3; i++)
		create_comp_codn_with_elem(&comp[i], &codn[i], &elem[i], 1);
	for (i = 0; i < 2; i++) {
		comp[i]->list.next = &comp[i + 1]->list;
		codn[i]->list.next = &codn[i + 1]->list;
	}
	comp[2]->list.next = NULL;
	codn[2]->list.next = NULL;
	prot[0] = protein_create_with_compounds(&comp[0]->list);
	if (prot[0] == NULL)
		die("incrementer_create: can't create prot0.");
	codn_head.next = &codn[0]->list;
	codn_tail.next = &codn[2]->list;

	/* protein1: REX Prefix=0x48, Opcode=0xff, Operand(ModR/M)=0xc0 */
	elem[0] = 0x48; elem[1] = 0xff; elem[2] = 0xc0;
	for (i = 0; i < 3; i++)
		create_comp_codn_with_elem(&comp[i], &codn[i], &elem[i], 1);
	for (i = 0; i < 2; i++) {
		comp[i]->list.next = &comp[i + 1]->list;
		codn[i]->list.next = &codn[i + 1]->list;
	}
	comp[2]->list.next = codn[2]->list.next = NULL;
	prot[1] = protein_create_with_compounds(&comp[0]->list);
	if (prot[1] == NULL)
		die("incrementer_create: can't create prot1.");
	codn_tail.next->next = &codn[0]->list;
	codn_tail.next = &codn[2]->list;

	/* protein2: Opcode=0xc3 */
	elem[0] = 0xc3;
	create_comp_codn_with_elem(&comp[0], &codn[0], &elem[0], 1);
	comp[0]->list.next = codn[0]->list.next = NULL;
	prot[2] = protein_create_with_compounds(&comp[0]->list);
	if (prot[2] == NULL)
		die("incrementer_create: can't create prot2.");
	codn_tail.next->next = &codn[0]->list;

	/* prot0 -> prot1 -> prot2 */
	prot[0]->list.next = &prot[1]->list;
	prot[1]->list.next = &prot[2]->list;
	prot[2]->list.next = NULL;

	/* 細胞にタンパク質のリストとDNAを設定 */
	cell->prot_head.next = &prot[0]->list;
	cell->codn_head.next = codn_head.next;

	/* 細胞にその他の設定を行う */
	cell->list.next = NULL;
	cell->life_duration = INCREMENTER_LIFE_DURATION;
	cell->life_left = cell->life_duration;

	return cell;
}

struct singly_list *incrementer_create_essential_compounds(void)
{
	element_t code[] = {
		0x48, 0x89, 0xf8,
		0x48, 0xff, 0xc0,
		0xc3
	};

	struct singly_list *comp_1st_entry;
	struct singly_list *comp_prev;

	unsigned int i;
	for (i = 0; i < NUM_COMPOUNDS; i++) {
		struct compound *comp =
			compound_create_with_elements(&code[i], 1);
		if (comp == NULL)
			die("incrementer_c_e_c: can't create a compound.");

		if (i == 0)
			comp_1st_entry = &comp->list;
		else
			comp_prev->next = &comp->list;

		comp_prev = &comp->list;
	}
	comp_prev->next = NULL;

	return comp_1st_entry;
}
