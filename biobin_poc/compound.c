#include <compound.h>

#define MAX_POOL_COMPOUNDS	100

static struct compound compound_pool[MAX_POOL_COMPOUNDS];
static unsigned int is_compound_creation;

void compound_pool_init(void)
{
	unsigned int i;
	for (i = 0; i < MAX_POOL_COMPOUNDS; i++)
		compound_pool[i].in_use = FALSE;
}

struct compound *compound_create(void)
{
	spin_lock(&is_compound_creation);

	unsigned int i;
	for (i = 0; i < MAX_POOL_COMPOUNDS; i++) {
		if (compound_pool[i].in_use == FALSE) {
			compound_pool[i].in_use = TRUE;
			spin_unlock(&is_compound_creation);
			compound_pool[i].list.next = NULL;
			compound_pool[i].len = 0;
			return &compound_pool[i];
		}
	}

	spin_unlock(&is_compound_creation);
	return NULL;
}

struct compound *compound_create_with_elements(
	element_t *elem_arry, unsigned int elem_len)
{
	struct compound *comp = compound_create();
	if (comp == NULL)
		return NULL;

	memcpy(comp->elements.bytes, elem_arry, elem_len);
	comp->len = elem_len;
	return comp;
}

struct compound *compound_create_with_data(bio_data_t data)
{
	struct compound *comp = compound_create();
	if (comp == NULL)
		return NULL;

	comp->elements.data = data;
	comp->len = sizeof(bio_data_t);
	return comp;
}

void compound_dump_entry(struct compound *comp)
{
	if (compound_is_data(comp) == TRUE) {
		putchar('[');
		puth(comp->elements.data ,16);
		putchar(']');
	} else {
		putchar('<');
		unsigned long long i;
		for (i = 0; i < comp->len; i++) {
			puth(comp->elements.bytes[i], 2);
			if (i < (comp->len - 1))
				putchar(' ');
		}
		putchar('>');
	}
}

bool_t compound_is_data(struct compound *comp)
{
	if (comp->len == sizeof(bio_data_t))
		return TRUE;
	return FALSE;
}

void compound_dump_list(struct singly_list *list_head, enum comp_filter filter)
{
	struct singly_list *entry;
	unsigned char is_first = TRUE;
	for (entry = list_head->next; entry != NULL; entry = entry->next) {
		struct compound *comp = (struct compound *)entry;

		if ((filter == COMP_FILTER_CODE)
		    && (compound_is_data(comp) == TRUE))
			continue;

		if ((filter == COMP_FILTER_DATA)
		    && (compound_is_data(comp) == FALSE))
			continue;

		if (is_first == TRUE)
			is_first = FALSE;
		else
			putchar(',');

		compound_dump_entry(comp);
	}
}
