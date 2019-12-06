#include <protein.h>

#define MAX_POOL_PROTEINS	100

static struct protein protein_pool[MAX_POOL_PROTEINS];
static unsigned int is_protein_creation;

void protein_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++)
		protein_pool[i].in_use = FALSE;
}

struct protein *protein_create(void)
{
	spin_lock(&is_protein_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_PROTEINS; i++) {
		if (protein_pool[i].in_use == FALSE) {
			protein_pool[i].in_use = TRUE;
			spin_unlock(&is_protein_creation);
			protein_pool[i].list.next = NULL;
			protein_pool[i].comp_head.next = NULL;
			return &protein_pool[i];
		}
	}

	spin_unlock(&is_protein_creation);
	return NULL;
}

struct protein *protein_create_with_compounds(
	struct singly_list *comp_1st_entry)
{
	struct protein *prot = protein_create();
	if (prot == NULL)
		return NULL;

	prot->comp_head.next = comp_1st_entry;
	return prot;
}

unsigned int protein_bond_compounds(struct protein *prot, unsigned char *buf)
{
	unsigned int len = 0;

	struct singly_list *entry;
	for (entry = prot->comp_head.next; entry != NULL; entry = entry->next) {
		struct compound *comp = (struct compound *)entry;
		memcpy(buf, comp->elements.bytes, comp->len);
		len += comp->len;
		buf += comp->len;
	}

	return len;
}

void protein_dump_entry(struct protein *prot)
{
	if (prot->comp_head.next == NULL)
		return;

	struct singly_list *entry;
	for (entry = prot->comp_head.next; entry != NULL; entry = entry->next) {
		compound_dump_entry((struct compound *)entry);
		if (entry->next != NULL)
			putchar('-');
	}
}

void protein_dump_list(struct singly_list *prot_head)
{
	struct singly_list *entry;
	for (entry = prot_head->next; entry != NULL; entry = entry->next) {
		protein_dump_entry((struct protein *)entry);
		if (entry->next != NULL)
			putchar(',');
	}
}
