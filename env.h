#ifndef ENV_H
#define ENV_H

#include <map>
#include <string>
#include <iostream>

using namespace std;

class ASTNode;

typedef map<string, ASTNode*> Frame;

class Environment 
{
  public:
    Environment(Environment *outer=0, const char *name=0): outer_(outer)
    {
      if (name != 0)
        name_ = string(name);
    }
    Environment *outer_;

    ASTNode* lookup(const string &var)
    {
      return lookup_by_env(this, var);
    }


    bool add(const string var, ASTNode *node)
    {
      auto count = frame_.count(var);
      cout << "frame_.count(var): " << count << endl;
      if (count > 0) // var exists
        return false;

      frame_.insert({var, node});
      return true;
    }

    Frame &frame()
    {
      return frame_;
    }


    string name_; // env name for debug
    int free_frame_index_;
  private:
    ASTNode* lookup_by_env(const Environment *env, const string &var)
    {
      auto it = env->frame_.find(var);
      if (it != env->frame_.end()) // find it
      {
        return it->second;
      }
      else
      {
        if (env->outer_)
          return lookup_by_env(env->outer_, var);
        else
          return nullptr;
      }
    }

    Frame frame_;
};

void create_primitive_procedure(Environment *env);


#endif
