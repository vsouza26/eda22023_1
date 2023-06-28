#include "trie.h"
#include "pattern.h"
#include <stdio.h>
#include <stdlib.h>

trie *trie_create(){
  trie *t = malloc(sizeof(trie));
  trie_node *t_n = _trie_create_node(NO_SYMBOL);
  t->head = t_n;
  if (t) return t;
  else return NULL;
}

trie_node *_trie_create_node(pattern_symbol ps){
  trie_node *tn = malloc(sizeof(trie_node));
  if(tn == NULL){
    handle_error(ERROR_TRIE, "Erro ao tentar criar árvore");
    return NULL;
  }
  tn->parent = NULL;
  for (int i = 0; i < GRAMMAR_SIZE; i++){
    tn->filhos[i] = NULL;
  }
  tn->p_s = ps;
  tn->dict_pos = -1;
  return tn;
}

void trie_add_pattern(trie *t, pattern *p){
  if (!pattern_is_valid(p)) handle_error(ERROR_TRIE,"Erro ao tentar adicionar padrão inválido a árvore");
  pattern_node *p_n = p->begin;
  trie_node *t_n = t->head;
  while (p_n) {
    if(t_n->filhos[p_n->p]){
      t_n = t_n->filhos[p_n->p];
      p_n = p_n->next;
    } else {
      trie_node *t_n_novo = _trie_create_node(p_n->p);
      t_n->filhos[p_n->p] = t_n_novo;
      t_n_novo->parent = t_n;
      t_n = t_n_novo;
      p_n = p_n->next;
    }
  }
  t_n->end = true;
}

trie_node *trie_add_pattern_return_last_elem_att_dict_pos(trie *t, pattern *p, int pos){
  if (!pattern_is_valid(p)) return NULL;
  pattern_node *p_n = p->begin;
  trie_node *t_n = t->head;
  while (p_n) {
    if(t_n->filhos[p_n->p]){
      t_n = t_n->filhos[p_n->p];
      p_n = p_n->next;
    } else {
      trie_node *t_n_novo = _trie_create_node(p_n->p);
      t_n->filhos[p_n->p] = t_n_novo;
      t_n_novo->parent = t_n;
      t_n = t_n_novo;
      p_n = p_n->next;
    }
  }
  t_n->end = true;
  t_n->dict_pos = pos;
  return t_n;
}

bool trie_exists_pattern(trie *t, pattern *p){
  pattern_node *p_n = p->begin;
  trie_node *t_n = t->head;
  while(true){
    if(t_n == NULL){ printf("t_n == null\n"); return false;}
    if(!p_n) { printf("p_n igual a null\n"); return false;}
    printf("Comparando t: %d com p_n:%d\n", t_n->p_s, p_n->p);
    if(t_n->p_s != NO_SYMBOL && t_n->p_s != p_n->p){ printf("simbolo diferente\n"); return false;}
    if(t_n->end && p_n->next == NULL) return true;
    if(t_n->p_s != NO_SYMBOL){
      p_n = p_n->next;
      if(!p_n) return false;
    }
    t_n = t_n->filhos[p_n->p];
  }
}

pattern *trie_retrieve_pattern(trie_node *t_n){
  pattern *p = pattern_create();
  trie_node *t_n_aux = t_n;
  while(t_n_aux){
    if(t_n_aux->p_s!=NO_SYMBOL)
      pattern_shove(p, t_n_aux->p_s);
    t_n_aux = t_n_aux->parent;
  }
  return p;
}

void trie_list(trie_node *t_n){
  if(!t_n) return;
  if(t_n->p_s!=NO_SYMBOL)
    printf("Node:%i is end: %i\n", t_n->p_s, t_n->end);
  for(int i = 0; i < GRAMMAR_SIZE; i++){
    trie_list(t_n->filhos[i]);
  }
}


