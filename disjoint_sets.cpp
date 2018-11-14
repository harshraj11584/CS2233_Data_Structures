#include <bits/stdc++.h>
using namespace std;
int num_nodes; //num_nodes is the total number of elements

struct node
{
	int rank =-1;	int val=-1;	struct node* parent=NULL;
};

struct node* make_set( int num )
{
	struct node* t = new node;
	t->val = num;	t->rank=1;	t->parent = t;
	return t;
}

struct node* get_parent(struct node* t)
{
	while(t->parent->val!=t->val)		t=t->parent;
	return t;
}

void do_union(struct node* point_to_node[], int a, int b)
{
	struct node *node1, *node2;
	struct node  *p_a = get_parent(point_to_node[a-1]), *p_b = get_parent(point_to_node[b-1]);
	if (p_a->rank >= p_b->rank)	{		node1=p_a; node2=p_b;	}
	else						{		node1=p_b; node2=p_a;	}
	node2->parent=node1;
	if (node1->rank == node2->rank)	node1->rank = node1->rank + 1;
}

int handle_operations()
{
	cin>>num_nodes;	struct node* point_to_node[num_nodes];
	for(int i=0;i<num_nodes;i++)
		point_to_node[i] = make_set((i+1));
	//array with pointers to all nodes has been created
	//all nodes have been initialized as singleton sets

	char ch=' ';
	while ((ch=fgetc(stdin))!=EOF && ch!='N')
	{

		if (ch=='?')
		{
			int a,b;	cin>>a>>b;
			if (a<1 || a>num_nodes || b<1 || b>num_nodes )	cout<<"-1\n";
			else
			{
				struct node* p_a = get_parent(point_to_node[a-1]);
				struct node* p_b = get_parent(point_to_node[b-1]);
				if (p_a->val == p_b->val)	cout<<"1\n";
				else	cout<<"0\n";
			}
		}
		else if (ch=='S')
		{
			int a; cin>>a;
			if (a>=1 && a<=num_nodes)	cout<<get_parent(point_to_node[a-1])->val<<"\n";
			else	cout<<"-1\n";
		}
		else if (ch=='R')
		{
			int a; cin>>a;
			if (a>=1 && a<=num_nodes)	cout<<point_to_node[a-1]->rank<<"\n";
			else	cout<<"-1\n";
		}
		else if (ch=='U')
		{
			int a,b; cin>>a>>b;
			if (a>=1 && a<=num_nodes && b>=1 && b<=num_nodes)	do_union(point_to_node, a, b);			
		}
	}

	//freeing the used memory for list here
	for(int i=0;i<num_nodes;i++)
	{
		struct node *t1 = point_to_node[i];
		point_to_node[i]=NULL;
		delete t1;
	}

	if (ch=='N')	return 1;
	else	return -1;
}

int main()
{
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