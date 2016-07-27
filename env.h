#ifndef ENV_H
#define ENV_H

#include "mytype.h"

#include <map>
#include <string>
#include <iostream>

using namespace std;

class ASTNode;

typedef map<string, ASTNode*> Frame;

class Environment 
{
  public:
    Environment(Environment *outer, const string &name): outer_(outer), name_(name)
    {
      id_ = count_;
      name_ = name + "##" + to_string(id_);
      ++count_;
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

    const string &name() const
    {
      return name_;
    }
    int free_frame_index_;

    bool edit(const string &str, ASTNode *node) 
    {
      cout  << "ss edit" << endl;
      frame_[str] = node;
      return true;
    }

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
    string name_; // env name for debug
    static u32 count_;
    u32 id_;
};

void create_primitive_procedure(Environment *env);
Environment *get_global_env();


#endif
