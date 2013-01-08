#include <iostream>
#include <cerrno>
#include <cstring>

#define OPTION_LEVEL      true
#define OPTION_NODE       false
#define OPTION_COMPLETE   true
#define OPTION_INCOMPLETE false

using namespace std;

typedef struct Node{
  int value_;
  struct Node * left_, * right_;
  Node(){
    this->value_ = 0;
    this->left_ = NULL;
    this->right_ = NULL;
  }
} node;


class Binary_Tree{

protected:
  unsigned int levels_;
  unsigned int nodes_num_;
  bool         level_node_;
  bool         complete_;
  node *       tree_root_;

public:
  Binary_Tree( unsigned int arg_levels_ = 0, 
	       unsigned int arg_nodes_num_ = 0,
	       bool arg_level_node_ = OPTION_LEVEL,
	       bool arg_complete_ = OPTION_COMPLETE ){
    this->levels_ = arg_levels_;
    this->nodes_num_ = arg_nodes_num_;
    this->level_node_ = arg_level_node_;
    this->complete_ = arg_complete_;
    this->tree_root_ = NULL;
  }

  virtual int construct_() = 0;
  virtual void print_() = 0;
};

class Ordinary_Binary_Tree : public Binary_Tree{

public:
  Ordinary_Binary_Tree( unsigned int arg_levels_ = 0, 
		       unsigned int arg_nodes_num_ = 0,
		       bool arg_level_node_ = OPTION_LEVEL,
		       bool arg_complete_ = OPTION_COMPLETE )
    : Binary_Tree( arg_levels_,
		   arg_nodes_num_,
		   arg_level_node_,
		   arg_complete_ ) {
  }

  int construct_();
  void print_();
};

int Ordinary_Binary_Tree :: construct_(){
  node * temp_root_ = NULL;
  if( this->complete_ ){
    if( this->level_node_ == OPTION_LEVEL )
      this->nodes_num_ = ( unsigned int )( 1 << this->levels_ ) - 1;
    else{
      unsigned int temp_nodes_num_ = this->nodes_num_;
      this->levels_ = 0;
      for( temp_nodes_num_; temp_nodes_num_ > 0; temp_nodes_num_ >> 1 )
	this->levels_++;
    }
    temp_root_ = ( node * )malloc( this->nodes_num_*sizeof(node) );
    if( errno != 0 )
      cout << "Error: " << strerror(errno) << endl;
    unsigned int temp_nodes_count_ = 0;
    for( int i = 0; i < this->levels_-1; i++ ){
      for( int j = 0; (j < (1 << i)) && (temp_nodes_count_ < this->nodes_num_); j++ ){
	temp_root_[i].left_ = temp_root_ + 2 * i;
	temp_root_[i].right_ = temp_root_ + 2 * i + 1;
      }
    }
  }
  this->tree_root_ = temp_root_;
  return 0;
}

void Ordinary_Binary_Tree :: print_(){
  cout << "Dummy print_() function." << endl;
  cout << "BFS to be implemented." << endl;
}

int main( int argc, char ** argv ){
  return 0;
}
