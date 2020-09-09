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
	if (head == NULL) { printf("No Using WP\n"); return; }
	if (h->NO == wp->NO) {head = head->next; return;}
	while (h->next != NULL && h->next->NO != wp->NO) h = h->next; 
	if (h->next == NULL) {printf( "No Found This WP\n"); return;}
	
	h->next = wp->next;
	p = free_;
        wp->next = NULL;
        if (p == NULL) p = wp;
        else {
                while (p->next != NULL) p = p->next;
                p->next = wp;
        }
	printf("Success Delete WatchPoint No.%d", wp->NO);
}
int delete_wp(int num){
	if (num >= 0 && num < NR_WP){
		WP tmp = wp_pool[num];
		free_WP(&tmp);
		return 1;
	}	
	return 0;
}
bool check_WP(){		
	
	WP *h;
	h = head;
	bool is;
	bool key = true;
	uint32_t NewValue;
	while (h != NULL) {
		NewValue = expr(h->str, &is);
		if (!is) Assert(0, "Calculate EXPR Wrong\n");
		if (NewValue != h->val){
			key = false;
			printf("Hit WatchPoint %d\n", h->NO);
			printf("Old Value Is %d, New Value is %d\n", h->val, NewValue);
			h->val = NewValue;
		}
		h = h->next;	
	}
	return key;
}
void print_wp(){
	WP *h = head;
	while (h != NULL)
	{
		printf("%d\t\t%s\t\t%d\n", h->NO, h->str, h->val);	
		h = h->next;	
	}
}



