#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <climits>
#include <stack>

using namespace std;

void disp(vector<string>& my)
{
    int n = my.size();
    for (int idx=0;idx<n;++idx)
    {
        cout<<my[idx]<<' ';
    }
    cout<<endl;
}

class TrieNode
{

public:
    unordered_map<char,TrieNode*> children;
    bool ends;

    TrieNode()
    {
        ends = false;
    }

    //function to insert a word in the trie
    void insert(string word)
    {
        TrieNode* cur = this;
        for (int idx=0;idx<word.length();++idx)
        {
            char c = word[idx];
            if (cur->children.find(c)==cur->children.end())
            {
                cur->children[c] = new TrieNode();
            }

            cur = cur->children[c];
        }

        cur->ends = true;
    }

    //search for a given word in the trie
    bool look(string word)
    {
        TrieNode* cur = this;
        for (int idx=0;idx<word.length();++idx)
        {
            char c = word[idx];
            if (cur->children.find(c)==cur->children.end()) return false;

            cur = cur->children[c];
        }

        return cur->ends;
    }

    //helper function to insert all the strings branching out from current node
    void complete(TrieNode* root,string cur,vector<string>& result)
    {
        if (root->ends)
        {
            result.push_back(cur);
        }


        auto it = root->children.begin();
        while (it!=root->children.end())
        {
            complete(it->second,cur+it->first,result);
            ++it;
        }


    }

    //returns a vector of possibilities
    vector<string> autocomplete(string query)
    {
        TrieNode* cur = this;
        vector<string> suggest;
        if (query=="") return suggest;
        for (int idx=0;idx<query.size();++idx)
        {
            char c = query[idx];
            if (cur->children.find(c)==cur->children.end()) return suggest;
            cur = cur->children[c];
        }

        complete(cur,query,suggest);

        return suggest;
    }
    //function to delete a word from trie
    void remove(string query)
    {
        stack<TrieNode*> path;
        TrieNode* cur = this;
        for (int idx=0;idx<query.size();++idx)
        {
            char c = query[idx];
            if (cur->children.find(c)==cur->children.end()) return;

            cur = cur->children[c];
            path.push(cur);
        }

        cur->ends = false;
        int idx=query.size();
        while (cur->children.size()==0)
        {
            delete cur;
            path.pop();
            path.top()->children.erase(query[idx-1]);
            --idx;
            cur = path.top();
        }
    }

};


int main()
{
    vector<string> words = {"see","bear","sell","stock","bull","buy","bid","hear","bell","stop"};
    TrieNode* myTrie = new TrieNode();

    for (int i=0;i<words.size();++i)
    {
        myTrie->insert(words[i]);
    }

    while (true)
    {
        cout<<"1: Insert a word"<<endl;
        cout<<"2: Query a word"<<endl;
        cout<<"3: Delete a word"<<endl;
        cout<<"-1: Exit"<<endl;

        unordered_set<int> ops = {1,2,3};
        int q;
        cin>>q;
        if (q==-1) break;
        if (ops.find(q)==ops.end())
        {
            cin.clear();
            cin.ignore(INT_MAX,'\n');
            cout<<"Invalid"<<endl;
            continue;
        }

        string w;
        cin>>w;

        if (q==1)
        {
            myTrie->insert(w);
            cout<<"Inserted "<<w<<endl;
        }
        else if(q==2)
        {
            vector<string> ans = myTrie->autocomplete(w);
            cout<<"suggestions: ";
            disp(ans);
        }
        else if(q==3)
        {
            myTrie->remove(w);
        }


    }

    return 0;
}
