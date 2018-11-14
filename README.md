## data_structures
Some Interesting Data Structures in C++


## 1. Red Black Tree: 

  A red–black tree is a kind of self-balancing binary search tree. 
	The balancing is good enough to guarantee searching in O(log n) time. 
	The insertion and deletion operations, along with the tree rearrangement and recoloring, 
	are also performed in O(log n) time.
  
Wiki:  https://en.wikipedia.org/wiki/Red-black_tree
  
Visualization: https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
  
• 'N a1 a2 a3 ... an' : creates new bst with these numbers
• '+ a' add element 'a' to set
• '> a' find successor of 'a'
• ‘– a’ delete element 'a'
• ‘S a’ search 'a' within most recent set, print path from root
• ‘C a’ print children of 'a'
• ‘P’   pre-order Traversal
  
## 2. Disjoint Sets:
    
A disjoint-set data structure is a data structure that tracks a set of elements partitioned 
into a number of disjoint subsets. It provides near-constant-time operations (bounded by the Inverse 
		Ackermann function) to add new sets, to merge existing sets, and to determine whether elements are 
		in the same set. Uses Union-by-Rank heuristic, guaranteeing worst case O(log n) for FindSet operations.
     
Wiki: https://en.wikipedia.org/wiki/Disjoint-set_data_structure
     
Ackermann Bounds: https://en.wikipedia.org/wiki/Ackermann_function
     
• ‘N n’ create singletons 1,2,3 ... n.
• ‘? a b’ tells whether a and b in same set or not.
• ‘S a’ prints representative of the set that holds 'a'.
• ‘U a b’ performs union of sets containing 'a' and 'b', uses Union by Rank Heuristic.
• ‘R b’ prints rank of 'b'.
     
## 3. BST that handles Infinitely Large numbers:
    
Like normal bst, but can handle infinitely large numbers. Memory handling verified using valgrind.
		
• ‘N’, ‘+’, ‘>’, ‘–’, ‘S’, ‘P’, 'C' same as above.
• ‘U a’ : Print Uncle
• ‘B a1 a2 a3 ... an’ : Build a new BST whose preorder traversal is a1 a2 ... an
• ‘L a’ and 'R a' : Left rotate and Right rotate at 'a' respecively
