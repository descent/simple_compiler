#include "env.h"

#if 0
Cell *proc_sub(Cell *cell)
Cell *proc_add(Cell *cell)
#endif


Environment *new_env(Environment *outer, const char *name)
{
  return new Environment(outer, name);
#if 0
  if (free_env_index >= MAX_ENVIRONMENT_POOL) return 0;
  
  Environment *env = &environment_pool[free_env_index++];
  env->outer_ = outer;
  env->free_frame_index_ = 0;
  strcpy(env->name_, name);
  return env;
#endif
}

void create_primitive_procedure(Environment *env)
{
}

#ifdef TEST_DEBUG
#include <stdio.h>
#include "astnode.h"

int main(int argc, char *argv[])
{
  ASTNode *a = (ASTNode*)0x1;
  ASTNode *b = (ASTNode*)0x2;

  Environment *env = new Environment(0, "globl_env"); 
  env->add("a", a);
  env->add("a", a);
  env->add("b", b);
  ASTNode* n = env->lookup("b");
  if (n)
    printf("n: %p\n", n);
  else
    printf("cannot found\n");

  Environment *f1_env = new Environment(env, "f1_env"); 
  ASTNode *fa = (ASTNode*)0x11;
  ASTNode *fb = (ASTNode*)0x22;
  f1_env->add("a", fa);
  f1_env->add("b", fb);

  n = f1_env->lookup("b");
  if (n)
    printf("n: %p\n", n);
  else
    printf("cannot found\n");
  return 0;
}
#endif
