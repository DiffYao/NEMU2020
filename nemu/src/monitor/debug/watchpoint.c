#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
	WP *w,*h;
	w = free_;
	if (w == NULL) {Assert(0, "No Free Watchpoint\n"); }
	free_ = free_->next;
	w->next = NULL;
	h = head;
	if (h == NULL) {head = w; head->next = NULL;}
	else {
		while (h->next != NULL) h = h->next;
		h->next = w; 
	}
	return w;
}

void free_WP(WP* wp){
	WP *p, *h;

	h = head;
	if (h == wp) {head = head->next; return;}
	while (h->next != NULL && h->next != wp) h = h->next; 
	if (h->next == NULL) Assert(0, "No Found This WP\n");
	
	h->next = wp->next;
	p = free_;
        wp->next = NULL;
        if (p == NULL) p = wp;
        else {
                while (p->next != NULL) p = p->next;
                p->next = wp;
        }

}




