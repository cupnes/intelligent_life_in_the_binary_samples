#pragma once

#include <compound.h>
#include <lib.h>

struct protein {
	/* Head */
	struct singly_list list;
	bool_t in_use;

	/* Compounds */
	struct singly_list comp_head;
};

void protein_pool_init(void);
struct protein *protein_create(void);
struct protein *protein_create_with_compounds(
	struct singly_list *comp_1st_entry);
unsigned int protein_bond_compounds(struct protein *prot, unsigned char *buf);
void protein_dump_entry(struct protein *prot);
void protein_dump_list(struct singly_list *prot_head);
