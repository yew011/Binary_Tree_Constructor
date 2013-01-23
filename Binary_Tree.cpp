#include <iostream>
#include <iomanip>
#include <queue>
#include <cerrno>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <list>

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
  virtual node * get_root_() = 0;
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
  node * get_root_();
};

int Ordinary_Binary_Tree :: construct_(){
  node * temp_root_ = NULL;
  // sanity check.
  if( ( this->level_node_ == OPTION_LEVEL &&
	this->levels_ == 0 ) || 
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
  temp_root_ = ( node * )malloc( (this->nodes_num_+1)*sizeof(node) );
  if( errno != 0 )
    cout << "Error: " << strerror(errno) << endl;
  for( int i = 0; i < this->nodes_num_+1; i++ )
    temp_root_[i].value_ = (int)'A'+i;
  // for complete tree.
  if( this->complete_ == OPTION_COMPLETE){
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
    unsigned int assign_idx_ = 1;
    bool break_cond_ = false;
    srand( time(NULL) );
    temp_list_[list_select_%2].push_back( &temp_root_[1] );
    cout << "put first." << endl;
    // node assignment logic.
    while( 1 ){
      for( list< node * >::iterator it = temp_list_[list_select_%2].begin();
	   it != temp_list_[list_select_%2].end(); ++it ){
	if( (rand() / (double) RAND_MAX) > 0.3 ){
	  if( (assign_idx_+1) == this->nodes_num_ ){
	    break_cond_ = true;
	    break;
	  }
	  assign_idx_++;
	  (*it)->left_ = temp_root_+assign_idx_;
	  temp_list_[(list_select_+1)%2].push_back(temp_root_+assign_idx_);
	}
	if( (rand() / (double) RAND_MAX) > 0.3 ){
	  if( (assign_idx_+1) == this->nodes_num_ ){
	    break_cond_ = true;
	    break;
	  }
	  assign_idx_++;
	  (*it)->right_ = temp_root_+assign_idx_;
	  temp_list_[(list_select_+1)%2].push_back(temp_root_+assign_idx_);
	}
      }
      if( break_cond_ ){
	break;
      }
      if( temp_list_[(list_select_+1)%2].size() != 0 ){
	temp_list_[list_select_%2].clear();
	list_select_++;
      }
    }
  }    
  this->tree_root_ = temp_root_;
  return 0;
}

void Ordinary_Binary_Tree :: print_(){
  list< node * > temp_list_[2];
  unsigned int list_select_ = 0;
  unsigned int assign_idx_ = 1;
  unsigned int temp_levels_ = this->levels_;
  bool break_cond_ = false;

  temp_list_[list_select_%2].push_back( this->tree_root_+1 );
  cout << "Tree value by level : " << endl;
  // the first level_.
  cout << setfill( ' ' )
       << setw( (temp_levels_-2) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) )
       << (char)this->tree_root_[1].value_ << endl;
  while( temp_levels_ > 1 ){
    temp_levels_--;
    for( list< node * >::iterator it = temp_list_[list_select_%2].begin(); ; ){
      if( (*it) != NULL ){
	if( (*it)->left_ != NULL ){
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ' << '/';
	} else {
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ' << ' ';
	}
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 0:(3*( (unsigned int)1<<(temp_levels_-2) )-1) )
	     << ' ';
	if( (*it)->right_ != NULL ){
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:(3*( (unsigned int)1<<(temp_levels_-2) )-1) ) << '\\';
	} else {
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:(3*( (unsigned int)1<<(temp_levels_-2) )-1) ) << ' ';
	}
      } else {
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ' << ' ';
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 0:(3*( (unsigned int)1<<(temp_levels_-2) )-1) )
	     << ' ';
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 1:(3*( (unsigned int)1<<(temp_levels_-2) )-1) ) << ' ';
      }
      if( (++it) == temp_list_[list_select_%2].end() ) break;
      else {
	cout << setfill( ' ' ) << setw( 3*(temp_levels_-1) ) << ' ' << ' ';
      }
    }
    cout << endl;
    // print the next level nodes.
    for( list< node * >::iterator it = temp_list_[list_select_%2].begin(); ; ){
      if( (*it) != NULL ){
	if( (*it)->left_ != NULL ){
	  temp_list_[(list_select_+1)%2].push_back( (*it)->left_ );
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << (char)(*it)->left_->value_;
	} else {
	  temp_list_[(list_select_+1)%2].push_back( NULL );
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ';
	}
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 3:3*( ((unsigned int)1<<(temp_levels_-2)) ) ) << ' ';
	if( (*it)->right_ != NULL ){
	  temp_list_[(list_select_+1)%2].push_back( (*it)->right_ );
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << (char)(*it)->right_->value_;
	} else {
	  temp_list_[(list_select_+1)%2].push_back( NULL );
	  cout << setfill( ' ' )
	       << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ';
	}
      } else {
	temp_list_[(list_select_+1)%2].push_back( NULL );
	temp_list_[(list_select_+1)%2].push_back( NULL );
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ';
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 3:3*( ((unsigned int)1<<(temp_levels_-2)) ) ) << ' ';
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ';
      }
      if( (++it) == temp_list_[list_select_%2].end() ) break;
      else {
	cout << setfill( ' ' )
	     << setw( (temp_levels_-1) == 0 ? 1:3*( (unsigned int)1<<(temp_levels_-2) ) ) << ' ';
      }
    }
    cout << endl;
    temp_list_[list_select_%2].clear();
    list_select_++;
  }
  cout << endl;
}

node * Ordinary_Binary_Tree :: get_root_(){
  return this->tree_root_+1;
}

int main( int argc, char ** argv ){
  Ordinary_Binary_Tree test_tree_(3,0);
  test_tree_.construct_();
  test_tree_.print_();
  Ordinary_Binary_Tree test_tree_1_(5,0);
  test_tree_1_.construct_();
  test_tree_1_.print_();
  Ordinary_Binary_Tree test_tree_2_(5,0,OPTION_LEVEL,OPTION_INCOMPLETE);
  test_tree_2_.construct_();
  test_tree_2_.print_();
  return 0;
}
