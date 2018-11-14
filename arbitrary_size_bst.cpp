#include <bits/stdc++.h>
#include <vector>
using namespace std;

//MA17BTECH11003
/* Program Allows Operating on Arbitrary Size Natural Numbers */
/*Choice in Node Deletion Implementation : Replacing with successor when deleting node*/
struct digitnode
{
	struct digitnode *next= NULL;
	struct digitnode *prev=NULL;
	int data=0;
};
struct bstnode
{
	struct bstnode *lc=NULL;
	struct bstnode *rc=NULL;
	struct bstnode *p=NULL;
	struct digitnode *h=NULL; //stores the pointer to head of the linked list of the number stored in this bstnode
};

struct bstnode *bstroot=NULL;

//Current count of numbers in bst is maintained. Later, No BST operation is called on an empty BST.
int count_of_nums_in_bst=0;
 
int compare(struct digitnode *head1, struct digitnode *head2)
//returns 1 when num1>num2, 0 when num1<num2, -1 when num1==num2
{
	unsigned long long int numberofdigits1=0,numberofdigits2=0;
	for(struct digitnode * t=head1;t!=NULL;t=t->next)
		numberofdigits1++;
	for(struct digitnode * t=head2;t!=NULL;t=t->next)
		numberofdigits2++;
	
	if (numberofdigits1>numberofdigits2)
				return(1);
	else if (numberofdigits1<numberofdigits2)
				return(0);
	else
	{			
		for (struct digitnode *t1=head1, *t2=head2; t1!=NULL; t1=t1->next, t2=t2->next)
		{
			if ( t1->data > t2->data )
			{
				return(1); break;
			}
			else if (t1->data < t2->data )
			{
				return(0); break;
			}
			if (t1->next==NULL && t2->next==NULL && t1->data==t2->data)
			{
				return(-1); break;
			}
		}
	}
}

void insert_in_bst(struct digitnode* head, struct bstnode* bstroot)
{
	if (bstroot->h==NULL)
		bstroot->h=head;
	else
	{
		if ((compare(bstroot->h,head))==1) //when bstroot->h is greater than
		{
			if (bstroot->lc==NULL)
			{
				struct bstnode *t=(struct bstnode *)malloc(sizeof(struct bstnode));
				bstroot->lc=t; t->p=bstroot;t->lc=NULL;t->rc=NULL;
				t->h=head;
				count_of_nums_in_bst++;
			}
			else
				insert_in_bst(head,bstroot->lc);
		}
		else if ((compare(bstroot->h,head))==0)//when bstroot->h is less than head
		{
			if (bstroot->rc==NULL)
			{
				struct bstnode *t=(struct bstnode *)malloc(sizeof(struct bstnode));
				bstroot->rc=t; t->p=bstroot;t->lc=NULL;t->rc=NULL;
				t->h=head;
				count_of_nums_in_bst++;
			}
			else
				insert_in_bst(head,bstroot->rc);
		}
	}
}

void deletenumberlist(struct digitnode *h)
//frees memory of the linked list storing number, and initializes head address to NULL
//also breaks all links such as prev, next before freeing memory used by node
{
	if (h!=NULL)
	{
		struct digitnode *t;
		while (h!=NULL)
		{
			t=h->next;
			h->data=0;
			h->next=NULL;
			h->prev=NULL;
			free(h);
			h=t;
		}
	}
}

void make_new_bst(struct bstnode* bstroot)
//makes a new bst of new set of numbers given by user, with bstroot address passed as argument
{
	char digit;
	struct digitnode *head=NULL, *cp=NULL;
	int firstdigit=1;
	//if first digit, then current digit being added to head of linked list, otherwise being appended to list in new node
	while((digit=fgetc(stdin))!='\n')
	{
		//the character read can be any one of a digit or a blankspace
		if (digit==' ')
		{
			if (head!=NULL)
				insert_in_bst(head,bstroot);
			head=NULL;
			head= (struct digitnode *)malloc(sizeof(struct digitnode));
			head->data=0;head->prev=NULL;head->next=NULL;
			firstdigit=1;
			cp=head;
		}
		else
		{
			if (!(firstdigit==1 && digit-48==0)) //this condition ignores initial extra zeroes in input number, if any
			{
				if (firstdigit==1)
				{
					//store current digit in head of first linked list
					cp=head;
					cp->data=digit-48;
					firstdigit=0;
					count_of_nums_in_bst++;
				}
				else
				{
					//current digit is not first digit
					//create new node to store this digit and add it after current node
					struct digitnode *t=(struct digitnode*)malloc(sizeof(struct digitnode));
					t->next=NULL;
					t->data=digit-48; //-48 for char digit to int value
					t->prev=cp;
					cp->next=t;
					cp=t;
				}
			}
		}	
	}
	//after the \n character is encountered and above loop exits, last number read has to be added to bst
	if (head!=NULL)
				insert_in_bst(head,bstroot);
	head=NULL;	
}

void preorder_traversal(struct bstnode* bstroot)
//traverses in preorder and prints numbers
{
	if (bstroot!=NULL && bstroot->h!=NULL)
	{
		struct digitnode *head=bstroot->h;
		for(struct digitnode *t=head;t!=NULL;t=t->next)
			cout<<t->data;
		cout<<" ";
		preorder_traversal(bstroot->lc);
		preorder_traversal(bstroot->rc);
	}
}

string search_in_bst(struct digitnode* searchhead, struct bstnode * bstnodeadd, string path)
//searches in bst, and path string variable maintains current path it has traversed while searching
{
	if (bstnodeadd==NULL)
		return ("-1");
	else if (compare(bstnodeadd->h,searchhead)==-1)
		return(path);
	else if (compare(bstnodeadd->h,searchhead)==1)
	{
		path=path+"0";
		path=search_in_bst(searchhead,bstnodeadd->lc,path);
		return path;
	}
	else
	{
		path=path+"1";
		path=search_in_bst(searchhead,bstnodeadd->rc,path);	
		return path;	
	}
}

void delete_old_bst(struct bstnode* bstadd)
//traverses the bst in postorder, first deletes and frees memory of linked list stored there, then deletes and frees bst node
//also breaks all links of bstnode such as parent, left child, right child
{
	if (bstadd!=NULL)
	{
		if (bstadd->lc!=NULL)
			{
				delete_old_bst(bstadd->lc);
				bstadd->lc=NULL;
			}
		if (bstadd->rc!=NULL)
			{
				delete_old_bst(bstadd->rc);
				bstadd->rc=NULL;
			}
		deletenumberlist(bstadd->h);
		bstadd->h=NULL;
		bstadd->p=NULL;
	}
	free(bstadd);
	bstadd=NULL;
}

struct digitnode* get_number()
//inputs a number, stores as linked list, and returns head
{
	struct digitnode* searchnumberhead = (struct digitnode*)malloc(sizeof(struct digitnode)), *cpt=searchnumberhead;
	searchnumberhead->prev=NULL;searchnumberhead->next=NULL;searchnumberhead->data=0; int firstdigit=1;	char n;
	while((n=fgetc(stdin))!='\n')
	{
		//the character read can be any one of a digit or a blankspace
		if (n>='0'&&n<='9')
		{
			if (!(firstdigit==1 && n-48==0)) //this condition ignores initial extra zeroes in input number, if any
			{
				if (firstdigit==1)
				{
					//store current digit in head of first linked list
					cpt=searchnumberhead; cpt->data=n-48; firstdigit=0;
				}
				else
				{
					//current digit is not first digit
					//create new node to store this digit and add it after current node
					struct digitnode *t=(struct digitnode*)malloc(sizeof(struct digitnode));
					t->next=NULL;
					t->data=n-48; //-48 for char digit to int value
					t->prev=cpt; cpt->next=t; cpt=t;
				}
			}
		}	
	}
	return searchnumberhead;
}

void remove_this_node(struct bstnode *address)
//this function simply removes the node whose address it is given
{
	if (address!=NULL)
	{
		if (address->p!=NULL && address->p->rc == address)
			address->p->rc=NULL;
		else if (address->p!=NULL && address->p->lc == address)
			address->p->lc=NULL;
		deletenumberlist(address->h);
		address->h=NULL;	address->p=NULL;address->lc=NULL;address->rc=NULL;
		free(address);
		address=NULL;
	}
}

void delete_node_from_bst(struct bstnode* address)
/*
if node has no children, remove it.
if node has 1 child, do splicing.
if node has 2 children, replace with successor and then remove the node.
*/
{
	if (address->lc==NULL && address->rc==NULL)
	{
		remove_this_node(address);
		
	}
	else if (address->lc==NULL && address->rc!=NULL)
	{
		if (address->p!=NULL)
		{
			if (address->p->lc==address)
			{
				address->p->lc=address->rc;
				address->rc->p=address->p;
			}
			else if (address->p->rc==address)
			{
				address->p->rc=address->rc;
				address->rc->p=address->p;
			}
			address->p=NULL; address->lc=NULL; address->rc=NULL;
			deletenumberlist(address->h); address->h=NULL; free(address); address=NULL;

		}
		else
		{
			bstroot=address->rc;
			address->rc->p=NULL;
			address->p=NULL; address->lc=NULL; address->rc=NULL;
			deletenumberlist(address->h); address->h=NULL; free(address); address=NULL;

		}
	}
	else if (address->lc!=NULL && address->rc==NULL)
	{
		if (address->p!=NULL)
		{
			if (address->p->lc==address)
			{
				address->p->lc=address->lc;
				address->lc->p=address->p;
			}
			else if (address->p->rc==address)
			{
				address->p->rc=address->lc;
				address->lc->p=address->p;
			}
			address->p=NULL; address->lc=NULL; address->rc=NULL;
			deletenumberlist(address->h); address->h=NULL; free(address); address=NULL;

		}
		else
		{
			bstroot=address->lc;
			address->lc->p=NULL;
			address->p=NULL; address->lc=NULL; address->rc=NULL;
			deletenumberlist(address->h); address->h=NULL; free(address); address=NULL;

		}
	}
	else if (address->lc!=NULL && address->rc!=NULL)
	{
		struct bstnode *address1;
		for (address1=address->rc;address1->lc!=NULL;address1=address1->lc);
		struct digitnode *successor=address1->h;
		address1->h=address->h;
		address->h=successor;
		delete_node_from_bst(address1);
	}
}

struct bstnode * inorder_traversal_till_successor_is_found(struct digitnode *num, struct bstnode* address, int found)
//modified inorder traversal till successor is found
//for faster traversal, we skip the left subtree of a node if the data of node is itself smaller than number whose successor is required
{
	if (address->lc!=NULL && compare(address->h,num)==1)
	{
		struct bstnode *succ=inorder_traversal_till_successor_is_found(num,address->lc,found);
		if (succ==NULL)
			return address;
		else
			return succ;
	}
	if (compare(address->h,num)==-1)
		found=1;
	if (found==1 && compare(address->h,num)!=-1)
		return address;
	if (found==0 && compare(address->h,num)==1)
		return address;
	if (address->rc!=NULL)
	{
		struct bstnode *succ=inorder_traversal_till_successor_is_found(num,address->rc,found);
		return succ;
	}
	return NULL;
}

struct bstnode * get_sibling(struct bstnode * address)
//this function returns the other child of a node's parent, if it exists, and NULL otherwise
{
	if (address->p==NULL)
		return NULL;
	else if (address->p->lc==address)
		return address->p->rc;
	else if (address->p->rc==address)
		return address->p->lc;
	else
		return NULL;
}

struct bstnode * point_to_address(struct bstnode *bstroot, string path)
//this function returns a bstnode pointer pointing to the node at given path
{
	if (path=="-1")
		return NULL;
	else if (path=="")
		return bstroot;
	else
	{
		struct bstnode * address=bstroot;			
		for (long long int i=0;i<path.length();i++)
		{
			if (path[i]=='0')	address=address->lc;
			else if (path[i]=='1')	address=address->rc;
		}	
		return address;
	}
}

void find_children()
{
	struct digitnode * num=get_number();
	string path = search_in_bst(num, bstroot, "");
	if (path=="-1")
		cout<<"-1\n";
	else
	{
		struct bstnode * address=point_to_address(bstroot, path);
		//address variable is now pointing at the node in the bst that contains this number;
		if (address->lc==NULL)
			cout<<"Nil";
		else
		{
			for(struct digitnode *t=address->lc->h;t!=NULL;t=t->next)
				cout<<t->data;
		}
		cout<<" ";
		if (address->rc==NULL)
			cout<<"Nil";
		else
		{
			for(struct digitnode *t=address->rc->h;t!=NULL;t=t->next)
				cout<<t->data;
		}
		cout<<"\n";
	}
	deletenumberlist(num);
}

void find_uncle()
{
	struct digitnode * num=get_number();
	string path = search_in_bst(num, bstroot, "");
	if (path=="-1")
		cout<<"-1\n";
	else
	{
		struct bstnode * address=point_to_address(bstroot, path);
		//address variable is now pointing at the node in the bst that contains this number;
		struct bstnode *uncle=NULL;
		if (address->p!=NULL)
			uncle=get_sibling(address->p);
		if (uncle==NULL)
			cout<<"-1";
		else
		{
			for(struct digitnode *t=uncle->h;t!=NULL;t=t->next)
				cout<<t->data;
		}
		cout<<"\n";
	}
	deletenumberlist(num);
}

void left_rotate()
{
	struct digitnode *numberhead=get_number();
	string path=search_in_bst(numberhead,bstroot,"");
	struct bstnode * address=point_to_address(bstroot, path);
	//address variable is now pointing at the node in the bst that contains this number;
	if (path!="-1")
		//this condition to ensure that left rotate is done only if this node is present
	{
		struct bstnode *R=address, *P=address->p, *A=R->lc, *B=R->rc, *T=NULL;
		if (R->rc!=NULL)
			T=R->rc->lc;
		/*
		Labelling:
		R is the node to be rotated, P is it's parent.
		A and B are lc and rc of R respectively.
		T is the left child of B.
		*/
		if (B!=NULL)
		{
			if (P!=NULL)
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
				{bstroot=B; B->p=NULL;}
			B->lc=R; R->p=B;
			R->rc=T; 
			if (T!=NULL)
				T->p=R;
			R->lc=A; 
			if (A!=NULL)
				A->p=R;
		}
	}
	deletenumberlist(numberhead);
}

void right_rotate()
{
	struct digitnode *numberhead=get_number();
	string path=search_in_bst(numberhead,bstroot,"");
	struct bstnode * address=point_to_address(bstroot, path);
		//address variable is now pointing at the node in the bst that contains this number;		

	if (path!="-1")
		//this condition to ensure that left rotate is done only if this node is present
	{
		struct bstnode *R=address, *P=address->p, *A=R->lc, *B=R->rc, *T=NULL;
		if (R->lc!=NULL)
			T=R->lc->rc;
		/*
		Labelling:
		R is the node to be rotated, P is it's parent.
		A and B are lc and rc of R respectively.
		T is the right child of A.
		*/
		if (A!=NULL)
		{
			if (P!=NULL)
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
				{bstroot=A; A->p=NULL;}
			A->rc=R; R->p=A;
			R->lc=T; 
			if (T!=NULL)
				T->p=R;
			R->rc=B; 
			if (B!=NULL)
				B->p=R;
		}
	}
	deletenumberlist(numberhead);
}

void take_input()
{
	char ch;
	while ((ch=fgetc(stdin))!=EOF)
	{
		if (ch=='N'||ch=='B')
		{
			count_of_nums_in_bst=0;
			delete_old_bst(bstroot); 
			bstroot=NULL; bstroot= (struct bstnode *)malloc(sizeof(struct bstnode));
			bstroot->lc=NULL;bstroot->rc=NULL;bstroot->p=NULL;bstroot->h=NULL;
			//cout<<"Old BST Deleted\n";
			make_new_bst(bstroot); 
			//cout<<"New BST Stored\n"<<"Current BST has "<<count_of_nums_in_bst<<" numbers\n";
		}
		else if (ch=='S')
		{
			struct digitnode *searchnumberhead=get_number();
			if (count_of_nums_in_bst>0)
			{
				struct bstnode* start_search_bstnode=bstroot;
				string path=search_in_bst(searchnumberhead,start_search_bstnode,"");
				cout<<path<<"\n";
				//cout<<"Search Completed\n";
			}
			else
				cout<<"-1\n";
			deletenumberlist(searchnumberhead);
			searchnumberhead=NULL;
			//cout<<"Linked List of Searched Number was Deleted\n";
		}
		else if (ch=='P')
		{
			if (count_of_nums_in_bst>0 && (bstroot!=NULL) && (bstroot->h!=NULL))
				preorder_traversal(bstroot); 
			cout<<"\n";
			//cout<<"Preorder Traversal Completed\n";
		}
		else if (ch=='+')
		{
			struct digitnode *addnumberhead=get_number();
			insert_in_bst(addnumberhead, bstroot);
			//cout<<"Inserted Number in BST\n";
		}
		else if (ch=='-')
		{
			struct digitnode *deletenumberhead=get_number();
			string path=search_in_bst(deletenumberhead,bstroot,"");
			struct bstnode * address=point_to_address(bstroot, path);
			//address variable is now pointing at the node in the bst that contains this number;
			if (path!="-1")
			delete_node_from_bst(address);
			deletenumberlist(deletenumberhead);
		}
		else if (ch=='>')
		{
			struct digitnode * num=get_number();
			struct bstnode * successor=inorder_traversal_till_successor_is_found(num, bstroot, 0);
			if (successor!=NULL)
			{
				for(struct digitnode *t=successor->h;t!=NULL;t=t->next)
					cout<<t->data;
				cout<<"\n";
			}
			else
				cout<<"-1\n";
			deletenumberlist(num);
		}
		else if (ch=='C')
		{
			find_children();
		}
		else if (ch=='U')
		{
			find_uncle();
		}
		else if (ch=='L')
		{
			left_rotate();
		}
		else if (ch=='R')
		{
			right_rotate();
		}
	}
}
int main()
{
	take_input();
	return 0;
}