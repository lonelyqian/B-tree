#include "BA.h"
#include <cstdlib> // Header file needed to use srand and rand
#include <ctime>
int f[1001];
int main()
{
    int node_num = 20;
    mydata test[100];
    int code[100];
    srand(time(0));
    for (int i = 0; i < node_num; i++)
        code[i] = i;
     
    for (int i = 0; i < node_num; i++)
        test[i].context = "root" + to_string(code[i]);
    BAtree tree;
    tree.create_();
    for (int i = 0; i < node_num; i++)
        tree.add_(code[i] , &test[i]);
    test_output(tree.root, 0);
    cout << "search interval:\n";
    cout << "input the interval:\n";
    int sear,sear2;
    cin >> sear >> sear2;
    cout << "the contexts with keys between sear and sear2\n";
    vector<mydata*> ans = tree.find_interval(sear, sear2);
    cout << ans.size() << endl;
    for (int i = 0; i < ans.size(); i++)
        cout << ans[i]->context << " ";
    node* p = tree.start;
    //test_output(tree.root, 0);
    
}
/*code[i] = rand() % 1000;
        while (f[code[i]])
        {
            code[i] = rand() % 1000;
        }
        cout << "input the data:(string)\n";
        cin >> test[99].context;
        f[code[i]] = 1;*/