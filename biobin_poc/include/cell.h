#pragma once

#include <compound.h>

#define DEFAULT_LIFE_DURATION	100

struct codon {
	/* Head */
	struct singly_list list;
	bool_t in_use;

	/* Data */
	bio_data_t comp_data;
	struct compound *buf;
};

struct cell {
	/* Head */
	struct singly_list list;
	bool_t in_use;

	/* Protein */
	struct singly_list prot_head;

	/* DNA */
	struct singly_list codn_head;

	/* Attributes */
	unsigned int life_duration;
	unsigned int life_left;
};

void cell_pool_init(void);
struct cell *cell_create(void);
struct codon *codon_create(void);
struct codon *codon_create_with_data(bio_data_t data);
bool_t cell_run(struct cell *cell);
void cell_dump_entry(struct cell *cell);
void cell_dump_list(struct singly_list *list_head);
