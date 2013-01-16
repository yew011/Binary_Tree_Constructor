#include <iostream>
#include <queue>
#include <cerrno>
#include <cstring>

#define OPTION_LEVEL      true
#define OPTION_NODE       false
#define OPTION_COMPLETE   true
#define OPTION_INCOMPLETE false

using namespace std;

/*
 * The Node structure, contains a integer value and points
 * to left- and right- child respectively.
*/
typedef struct Node{
  int value_;
  struct Node * left_, * right_;
  Node(){
    this->value_ = 0;
    this->left_ = NULL;
    this->right_ = NULL;
  }
} node;

/*
 * The base (abstract) class.
 * Instance Variables:
 *    - levels_       :  the number of levels of the tree;
 *    - nodes_num_    :  the number of nodes in the tree;
 *    - level_node_   :  boolean flag to select between using
 *                       level_ (true) or node_ (false) in the
 *                       tree construction;
 *    - complete_     :  boolean flag to select between complete
 *                       (tree) or incomplete_ (false) tree;
 *    - tree_root_    :  the node* pointing at the beginning of
 *                       the tree;
 *
*/
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


/*
 * Ordinary_Binary_Tree : derived class.
 *
 * The construct_ and print_ function is implemented.
 *
 * Examples:
 *         Ordinary_Binary_Tree complete_tree_();                     // will be valid, will not create anything,
 *                                                                    // the instance variable tree_root_ = NULL.
 *
 *         Ordinary_Binary_Tree complete_tree_(5,5);                  // will create a 5 level complete tree,
 *                                                                    // since the OPTION_LEVEL and OPTION_COMPLETE
 *                                                                    // are selected by default.
 *
 *         Ordinary_Binary_Tree complete_tree_(5,32,OPTION_NODE);     // will create a complete tree with 32 nodes.
 *
 *         Ordinary_Binary_Tree complete_tree_(5,32,OPTION_NODE,OPTION_INCOMPLETE);
 *                                                                    // will create an incomplete tree with 32 nodes.
 *                                                                    // the tree will be randomly built.
*/
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
  // sanity check.
  if( ( this->level_node_ == OPTION_LEVEL &&
	this->level_ == 0 ) || 
      ( this->level_node_ == OPTION_NODE &&
	this->nodes_num_ == 0 ) ){
	this->tree_root_ = temp_root_;
	return 0;
  }
  // calculate the number of nodes.
  if( this->level_node_ == OPTION_LEVEL )
    this->nodes_num_ = ( unsigned int )( 1 << this->levels_ ) - 1;
  else{
    unsigned int temp_nodes_num_ = this->nodes_num_;
    this->levels_ = 0;
    for( temp_nodes_num_; temp_nodes_num_ > 0; temp_nodes_num_ >> 1 )
      this->levels_++;
  }
  // malloc the nodes as an array.
  temp_root_ = ( node * )malloc( this->nodes_num_*sizeof(node)+1 );
  if( errno != 0 )
    cout << "Error: " << strerror(errno) << endl;
  // for complete tree.
  if( this->complete_ == OPTION_COMPLETE){
    for( int i = 0; i < this->nodes_num_+1; i++ )
      temp_root_[i].value_ = i;
    unsigned int temp_nodes_count_ = 0;
    for( int i = 1; i < ( (unsigned int)1 << (this->levels_-1) ); i++ ){
      if( 2*i <= this->nodes_num_ )
	temp_root_[i].left_ = temp_root_ + 2 * i;
      if( 2*i+1 <= this->nodes_num_ )
	temp_root_[i].right_ = temp_root_ + 2 * i + 1;
    }
  }
  // for incomplete tree.
  else{
    list< node * > temp_list_[2];
    unsigned int list_select_ = 0;
    list_select_[i].push_back( &temp_root_[1] );
    // node assignment logic.

  }
  this->tree_root_ = temp_root_;
  return 0;
}

void Ordinary_Binary_Tree :: print_(){
  queue< node * > temp_queue_;
  temp_queue_.push( this->tree_root_+1 );
  cout << "Tree value by level : " << endl;
  while( !temp_queue_.empty() ){
    if( temp_queue_.front() != NULL ){
      temp_queue_.push( (( node * )temp_queue_.front())->left_ );
      temp_queue_.push( (( node * )temp_queue_.front())->right_ );
      cout << (( node * )temp_queue_.front())->value_ << " ";
    }
    temp_queue_.pop();
  }
  cout << endl;
}

int main( int argc, char ** argv ){
  Ordinary_Binary_Tree test_tree_(3,0);
  test_tree_.construct_();
  test_tree_.print_();
  Ordinary_Binary_Tree test_tree_1_(5,0);
  test_tree_1_.construct_();
  test_tree_1_.print_();
  return 0;
}
