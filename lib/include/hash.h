#include <iostream>
#include <vector>
using namspeace std;

template<typename Key,typename Val>
class Node
{
	private:
		Key k;
		Val v;
		Node *next;
		Node *prev;
	public:
		Node(const Key index, const Val value):
		k(index), v(value), next(NULL), prev(NULL){}

		Key getKey()
		{
			return k;
		}

		Val getVal()
		{
			return v;
		}
}

template<typename Key, typename Val>
class HashTable
{
	private:
		vector<Node *> table;
		int size;
		int max_size;
	
	public:
		HashTable(): size(0),max_size(16){}


		int hashFunc(Key k)
		{
			int code = k % max_size;
			/*TODO*/

		}

		void insertNode(Node entry)
		{
			/*TODO*/
		}

		void removeNode(Node entry)
		{
			/*TODO*/
		}
	
		Node *findNode(Node entry)
		{
			/*TODO*/
		}

		int tableSize()
		{
			/*TODO*/
		}
			
		
}



