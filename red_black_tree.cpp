#include <bits/stdc++.h>
using namespace std;
struct rbt_node
{
	int val = -55;	//val = -55 indicates that this is sentinel node
	struct rbt_node *lc=NULL, *rc=NULL, *p=NULL;
	char color='B';
};
struct rbt_node *sentinel=NULL, *bstroot=NULL;

void add_sentinel_if_leaf(struct rbt_node *node)
//adds sentinel as lc and rc of node if node is a leaf and doesn't have sentinels
{	
	if (node!=NULL && node->val!=-55)
	{
		if (node->lc==NULL)
			node->lc=sentinel;
		if (node->rc==NULL)
			node->rc=sentinel;
	}
}

struct rbt_node * grandparent(struct rbt_node *node)
{
	if (node==NULL)
		return NULL;
	if (node->p!=NULL && node->p!=sentinel && node->p->p!=NULL && node->p->p!=sentinel)
		return node->p->p;

	return NULL;
}

struct rbt_node * uncle(struct rbt_node *node)
{
	if (node==NULL)
		return NULL;
	struct rbt_node *u=NULL;
	if (node->p!=NULL && node->p!=sentinel)
	{
		if (node->p->p!=NULL && node->p->p!=sentinel)
		{
			if (node->p->p->lc->val == node->p->val)
				u = node->p->p->rc;
			else 
				u = node->p->p->lc;
		}
	}
	if (u!=NULL)
		return u;	
	return NULL;
}

struct rbt_node * sibling(struct rbt_node *node)
{
	struct rbt_node *s=NULL;
	if (node==NULL)
		return NULL;
	if (node->p!=NULL && node->p!=sentinel)
	{
		if (node->p->lc->val == node->val)
			s=node->p->rc;
		else if (node->p->rc->val == node->val)
			s=node->p->lc;
	}
	if (s!=NULL)
		return s;
	return NULL;
}

void show_preorder( struct rbt_node * node )
//recursive function to print preorder traversal of a bst
{
	if (node!=NULL && node->val!=-55)
		cout<<node->val <<" ";

	if (node->lc!=NULL && node->lc->val!=-55)
		show_preorder(node->lc);
	if (node->rc!=NULL && node->rc->val!=-55)
		show_preorder(node->rc);
}

void leaves_preorder( struct rbt_node * node )
//assigns sentinel nodes to those nodes which are leaves but do not have sentinel attached
{
	if (node!=NULL && node->val!=-55)
		add_sentinel_if_leaf(node);
	if (node->val!=-55)
	{
		if (node->lc!=NULL && node->lc->val!=-55)
		leaves_preorder(node->lc);
		if (node->rc!=NULL && node->rc->val!=-55)
		leaves_preorder(node->rc);
	}
}

void recolor( struct rbt_node * node)
{
	if (node!=NULL)
	{
		if (node->color =='B')
			node->color='R';
		else if (node->color=='R')
			node->color='B';
	}
}

string find_path( struct rbt_node * add, int num, string path )
//recursive function that returns path of node in a bst, as a bitstring
{
	if (add==NULL || add->val == -55)
		return ("-1");
	else if (add->val == num)
		return(path);
	else if (add->val > num )
	{
		path=path+"0";
		path=find_path(add->lc, num, path);
		return path;
	}
	else
	{
		path=path+"1";
		path=find_path(add->rc, num, path);	
		return path;	
	}
}

void left_rotate(struct rbt_node *address)
{
	leaves_preorder(bstroot);
	//address variable is now pointing at the node in the bst that contains this number;
	if (address!=NULL)
		//this condition to ensure that left rotate is done only if this node is present
	{
		struct rbt_node *R=address, *P=address->p, *A=R->lc, *B=R->rc, *T=NULL;
		if (R->rc!=NULL)
			T=R->rc->lc;
		/*
		Labelling:
		R is the node to be rotated, P is it's parent. A and B are lc and rc of R respectively. T is the left child of B.
		*/
		if (B!=NULL)
		{
			if (P!=NULL && P!=sentinel)
			{
				if (P->rc==address)
				{
					P->rc=B; B->p=P;
				}
				else if (P->lc==address)
				{
					P->lc=B; B->p=P;
				}
			}
			else
				{
					bstroot=B; B->p=sentinel;	//cout<<"Changed Root\n";
				}
			B->lc=R; R->p=B;
			R->rc=T; 
			if (T!=NULL)
				T->p=R;
			R->lc=A; 
			if (A!=NULL)
				A->p=R;
		}
	}
	leaves_preorder(bstroot);
}

void right_rotate(struct rbt_node *address)
{
	leaves_preorder(bstroot);

	//address variable is now pointing at the node in the bst that contains this number;	
	if (address!=NULL)
		//this condition to ensure that left rotate is done only if this node is present
	{
		struct rbt_node *R=address, *P=address->p, *A=R->lc, *B=R->rc, *T=NULL;
		if (R->lc!=NULL)
			T=R->lc->rc;

		/*
		Labelling:
		R is the node to be rotated, P is it's parent.A and B are lc and rc of R respectively.T is the right child of A.
		*/
		if (A!=NULL)
		{
			if (P!=NULL && P!=sentinel)
			{
				if (P->rc==address)
				{
					P->rc=A; A->p=P;
				}
				else if (P->lc==address)
				{
					P->lc=A; A->p=P;
				}
			}
			else
				{
					bstroot=A; A->p=sentinel;	//cout<<"Changed Root\n"; cout<<"BST root is "<<bstroot->val<<"\n";
				}
			A->rc=R; R->p=A;
			R->lc=T; 
			if (T!=NULL)
				T->p=R;
			R->rc=B; 
			if (B!=NULL)
				B->p=R;
		}

	}
	leaves_preorder(bstroot);
}

void fix_insert(struct rbt_node *node)
//fixes the insertion of node in the rbt whose root is bstroot
{
	//Property II : If Root Node is not black, then node itself is root	
	if (bstroot->color == 'R' && bstroot->val == node->val)
	{
		recolor(bstroot);
		//cout<<"Fixed violation of property 2\n";
	}

	//Property IV : Children of red node are black
	if (node->color=='R' && node->p!=NULL && node->p!=sentinel && node->p->color=='R')
		//P4 violated by node and its parent
	{
		//cout<<"P4 violated\n";
		struct rbt_node *uncle_t = uncle(node);
		struct rbt_node *grandparent_t = grandparent(node);
		struct rbt_node *parent_t = node->p;

		//Case I : Uncle of node is Red
		if (uncle_t!=NULL && uncle_t->color=='R')
		{			
			recolor(parent_t);
			recolor(uncle_t);
			recolor(grandparent_t);
			//cout<<"Fixed Violation P4 Case 1\n";
			fix_insert(grandparent_t);			
		}

		//Case III : Uncle of node is Black and node is left child of a left child
		else if (uncle_t!=NULL && uncle_t->color == 'B' && parent_t!=NULL && parent_t->lc->val==node->val && grandparent_t!=NULL && grandparent_t->lc->val==parent_t->val )
		{
			recolor(parent_t); 
			recolor(grandparent_t);
			right_rotate(grandparent_t);
			//cout<<"Fixed Violation P4 Case 3\n";
		}

		//Case III Symmetry : Uncle of node is Black and node is right child of a right child
		else if (uncle_t!=NULL && uncle_t->color == 'B' && parent_t!=NULL && parent_t->rc->val==node->val && grandparent_t!=NULL && grandparent_t->rc->val==parent_t->val )
		{
			recolor(parent_t); 
			recolor(grandparent_t);
			left_rotate(grandparent_t);
			//cout<<"Fixed Violation P4 Case 3\n";
		}

		//Case II : Uncle of node is Black and node is right child of a left child
		else if (uncle_t!=NULL && uncle_t->color == 'B' && parent_t!=NULL && parent_t->rc->val==node->val && grandparent_t!=NULL && grandparent_t->lc->val==parent_t->val )
		{
			node=parent_t;
			uncle_t=uncle(node);
			parent_t=node->p;
			grandparent_t=grandparent(node);
			left_rotate(node);
			fix_insert(node);
			//cout<<"Fixed Violation P4 Case 2\n";
		}

		else if (uncle_t!=NULL && uncle_t->color == 'B' && parent_t!=NULL && parent_t->lc->val==node->val && grandparent_t!=NULL && grandparent_t->rc->val==parent_t->val )
		{
			node=parent_t;
			uncle_t=uncle(node);
			parent_t=node->p;
			grandparent_t=grandparent(node);
			right_rotate(node);
			//cout<<"Fixed Violation P4 Case 2\n";
			fix_insert(node);			
		}
	}
}

void insert_normally_in_bst(struct rbt_node *node, struct rbt_node* address)
{
	if (address->val==-55)
		{
			//cout<<"Even after inserting at bstroot, val at bstroot is -55\n";
			address=node; address->lc=sentinel; address->rc=sentinel;
		}

	else if (address->val > node->val)
	{
		if (address->lc->val == -55)
		{
			address->lc = node; node->p=address; node->lc=sentinel; node->rc=sentinel;
		}
		else
			insert_normally_in_bst(node,address->lc);
	}
	else if (address->val < node->val)
	{
		if (address->rc->val==-55)
		{
			address->rc = node; node->p=address; node->lc=sentinel; node->rc=sentinel;
		}
		else
			insert_normally_in_bst(node,address->rc);
	}
}

void insert_in_rbt ( int num)
{
	//cout<<"Insert called for "<<num<<" With bstroot->val = "<<bstroot->val<<"\n";
	if (bstroot->lc!=NULL && bstroot->rc!=NULL)
	{
		//cout<<"bstroot->lc: "<<bstroot->lc->val<<" bstroot->rc: "<<bstroot->rc->val<<"\n";
	}	
	if (bstroot->val == -55)
		//rbt is empty with just a sentinel node
	{
		//cout<<"Inserting at root\n";
		bstroot->val = num; bstroot->p=sentinel; bstroot->lc=sentinel; bstroot->rc=sentinel; bstroot->color='R';
		//cout<<"Inserted.\n"; show_preorder(bstroot); cout<<"\nNow fixing\n";
		fix_insert(bstroot);
		//cout<<"BST root is "<<bstroot->val<<"\n"; show_preorder(bstroot); cout<<"\n";
	}
	else
	{
		struct rbt_node *node = new rbt_node();
		node->val = num; node->lc=NULL; node->rc=NULL; node->p=NULL; node->color = 'R';

		insert_normally_in_bst(node,bstroot);
		//cout<<"Inserted.\n"; show_preorder(bstroot); cout<<"\nNow fixing\n";
		fix_insert(node);
		//cout<<"BST root "<<bstroot->val<<"\n"; show_preorder(bstroot); cout<<"\n";
	}
}

void handle_N()
//this functions inputs numbers separated by spaces and inserts them in BST
{
	string inp_nums; getline(cin,inp_nums);
	int curr_num=-55;
	string curr_num_ext="";
	for(int i=0;i<inp_nums.size();i++)
	{
		if (inp_nums[i]==' ')
		{
			if (curr_num_ext!="") 
			{
				curr_num = stoi(curr_num_ext);	//cout<<"Inserting "<<curr_num<<"\n";
				insert_in_rbt(curr_num);				
				curr_num_ext="";
				curr_num=-55;
			}
		}
		else if (inp_nums[i]=='\n')
		{
			if (curr_num_ext!="") 
			{
				curr_num = stoi(curr_num_ext);	//cout<<"Inserting "<<curr_num<<"\n";
				insert_in_rbt(curr_num);				
				curr_num_ext="";
				curr_num=-55;
			}
			break;
		}
		else if (inp_nums[i]!=' ' && inp_nums[i]!='\n')
		{
			curr_num_ext = curr_num_ext + inp_nums[i];
		}
		if (i==inp_nums.size()-1 && curr_num_ext!="")
		{
			curr_num = stoi(curr_num_ext); //cout<<"Inserting "<<curr_num<<"\n";
			insert_in_rbt(curr_num);			
			curr_num_ext="";
			curr_num=-55;
		}
	}
}

struct rbt_node *get_pred_for_deletion(struct rbt_node *node)
{
	if (node!=NULL)
	{
		struct rbt_node * res=node->lc;
		if (res->val!=-55)
		{
			while(res->rc!=NULL && res->rc->val!=-55)		res=res->rc;
		}
		if (res!=NULL && res->val!=-55)
		return res;
	}
	return NULL;	
}

struct rbt_node* find_successor(int num, struct rbt_node* address, int found)
//returns the pointer to node in rbt that conatins successor of the number num, and NULL otherwise
{
	if (address==NULL || address->val==-55)
		return NULL;
	if ( address->lc!=NULL && address->val > num )
	{
		struct rbt_node *succ=find_successor(num,address->lc,found);
		if (succ==NULL)
			return address;
		else
			return succ;
	}
	if ( address->val == num )
		found=1;
	if (found==1 && address->val != num)
		return address;
	if (found==0 && address->val > num)
		return address;
	if (address->rc!=NULL)
	{
		struct rbt_node *succ= find_successor(num,address->rc,found);
		return succ;
	}
	return NULL;
}

void transplant (struct rbt_node* u, struct rbt_node *v)
{
	if ( (u->p==sentinel || u->p==bstroot)  && !(u->p==sentinel && u->p==bstroot))
		bstroot=v;
	else if (u==u->p->lc)
		u->p->lc=v;
	else
		u->p->rc=v;
	v->p=u->p;
}

void delete_fixup(struct rbt_node *x)
{
	while (x!=bstroot && x->color=='B')
	{
		if (x==x->p->lc)
		{
			struct rbt_node *w=x->p->rc;
			if (w->color=='R')
			{
				w->color='B';
				x->p->color='R';
				left_rotate(x->p);	bstroot->p=sentinel;
				w=x->p->rc;
			}
			if (w->lc->color=='B' && w->rc->color=='B')
			{
				w->color='R';
				x=x->p;
			}
			else 
			{
				if (w->rc->color=='B')
				{
					w->lc->color='B';
					w->color='R';
					right_rotate(w); bstroot->p=sentinel;
					w=x->p->rc;
				}
				w->color=x->p->color;
				x->p->color='B';
				w->rc->color='B';
				left_rotate(x->p); bstroot->p=sentinel;
				x=bstroot;
			}
		}
		else
		{
			struct rbt_node *w=x->p->lc;
			if (w->color=='R')
			{
				w->color='B';
				x->p->color='R';
				right_rotate(x->p); bstroot->p=sentinel;
				w=x->p->lc;
			}
			if (w->rc->color=='B' && w->lc->color=='B')
			{
				w->color='R';
				x=x->p;
			}
			else 
			{
				if (w->lc->color=='B')
				{
					w->rc->color='B';
					w->color='R';
					left_rotate(w); bstroot->p=sentinel;
					w=x->p->lc;
				}
				w->color=x->p->color;
				x->p->color='B';
				w->lc->color='B';
				right_rotate(x->p); bstroot->p=sentinel;
				x=bstroot;
			}
		}
	}
	x->color='B';
}

void delete_node(struct rbt_node *z)
{
	struct rbt_node *y=z,*x=NULL;
	char y_original_color=y->color;
	if (z->lc->val == sentinel->val)
	{
		x=z->rc;
		transplant(z, z->rc);
	}
	else if (z->rc->val == sentinel->val)
	{
		x=z->lc;
		transplant(z,z->lc);
	}
	else
	{
		y=get_pred_for_deletion(z);
		y_original_color=y->color;
		x=y->lc;
		if (y->p==z)
			x->p=y;
		else
		{
			transplant(y,y->lc);
			y->lc=z->lc;
			y->lc->p=y;
		}
		transplant(z,y);
		y->rc=z->rc;
		y->rc->p=y;
		y->color=z->color;
	}
	if (y_original_color=='B')
		delete_fixup(x);
}

void postorder_deletion_of_entire_rbt(struct rbt_node* node)
{
	if (node->lc!=NULL && node->lc!=sentinel)
		postorder_deletion_of_entire_rbt(node->lc);
	if (node->rc!=NULL && node->rc!=sentinel)
		postorder_deletion_of_entire_rbt(node->rc);

	if (node!=NULL && node!=sentinel)
	{
		node->lc=NULL; node->rc=NULL; node->val=-55; node->p=NULL; node->color='B'; 
		delete(node);
		node=NULL;
	}
}

int handle_operations()
{
	//Creatin new BST with only sentinel node
	bstroot = new rbt_node();
	bstroot->val=-55; bstroot->lc=NULL; bstroot->rc=NULL; bstroot->p=sentinel; bstroot->color='B';
	
	//this functions inputs numbers separated by spaces and inserts them in BST
	handle_N();
	
	char ch=' ';
	while ((ch=fgetc(stdin))!=EOF && ch!='N')
	{		
		if (ch=='+')
		{
			int num; cin>>num;	//have to add num to rbt 
			//cout<<"inserting "<<num<<"\n";
			insert_in_rbt(num);
		}
		else if (ch=='>')
		{
			int num; cin>>num;
			//have to find successor of the number num
			struct rbt_node *succ = find_successor(num, bstroot, 0);
			if (succ!=NULL)
				cout<<succ->val<<"\n";
			else
				cout<<"-1\n";
		}
		else if (ch=='-')
		{
			bstroot->p=sentinel;
			leaves_preorder(bstroot);
			int num; cin>>num;
			//have to delete num from rbt
			string path = find_path(bstroot, num, "" );
			struct rbt_node * del_node=bstroot;
			if (path!="-1" && path!="")
			{			
				for (long long int i=0;i<path.length();i++)
				{
					if (path[i]=='0')	del_node=del_node->lc;
					else if (path[i]=='1')	del_node=del_node->rc;
				}		
			}
			//del_node now has pointer to node which has to be deleted
			delete_node(del_node); 	bstroot->p=sentinel;
		}
		else if (ch=='S')
		{
			int num; cin>>num;
			//have to search for num in bst
			//cout<<"Searching for "<<num<<" in Bst with root= "<<bstroot->val<<" \n";
			if (bstroot->val != -55 )
			{
				string path = find_path(bstroot, num, "" );
				if (path!="-1")
				{
					cout<<path<<" ";
					struct rbt_node * address=bstroot;
					if (path!="-1" && path!="")
					{			
						for (long long int i=0;i<path.length();i++)
						{
							if (path[i]=='0')	address=address->lc;
							else if (path[i]=='1')	address=address->rc;
						}		
					}
					//address now has pointer to node whose color are needed
					cout<<address->color<<"\n";
				}
				else
					cout<<path<<"\n";
			}
			else
				cout<<"-1\n";			
		}
		else if (ch=='C')
		{	
			int num; cin>>num;	//have to search for num in bst
			if (bstroot->val != -55 )
			{
				string path = find_path(bstroot, num, "");				
				struct rbt_node * address=bstroot;
				if (path!="-1" && path!="")
				{			
					for (long long int i=0;i<path.length();i++)
					{
						if (path[i]=='0')	address=address->lc;
						else if (path[i]=='1')	address=address->rc;
					}		
				}
				//address now has pointer to node whose children are needed
				if (address->lc!=NULL && address->lc->val!=-55)
					cout<<address->lc->val <<" "<<address->lc->color<<" ";
				else if (address->lc!=NULL)
					cout<<"L "<<address->lc->color<<" ";
				else
					cout<<"Error: lc of address is NULL while trying to print children of address\n";
				if (address->rc!=NULL && address->rc->val!=-55)
					cout<<address->rc->val <<" "<<address->rc->color<<"\n";
				else if (address->rc!=NULL)
					cout<<"L "<<address->rc->color<<"\n";
				else
					cout<<"Error: rc of address is NULL while trying to print children of address\n";
			}
			else
				cout<<"-1\n";
		}
		else if (ch=='P')
		{
			show_preorder(bstroot);
			cout<<"\n";
		}
	}	
	//free the used memory for red black tree here. 
	postorder_deletion_of_entire_rbt(bstroot);	bstroot=NULL;
	//Entire Tree is deleted here.	

	//deciding whether next operation is on another bst or terminating
	if (ch=='N')		return 1;
	else		return -1;
}

int main()
{
	sentinel = new rbt_node();
	sentinel->val = -55; sentinel->lc=NULL; sentinel->rc=NULL; sentinel->p=NULL; sentinel->color='B';
	char ch=' ';
	while ((ch=fgetc(stdin))!=EOF)
	{
		if (ch=='N')
		{
			int res=handle_operations();
			while (res==1)
			res=handle_operations();
		}
	}
	return 0;
}