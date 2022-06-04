#include "BA.h"
using namespace std;

void BAtree::create_()
{
    root = new node();
    root->num = 0;
    root->state = State::leaf_node;
    root->bro = nullptr;
    fa[root] = nullptr;
    fapos[root] = -1;

}
// δ����������÷���λ����Ϣ  ans �����Ҷ�ӽڵ�  rp ��Ҷ�ӽڵ��е�����λ��,���߸ò����λ��
mydata* BAtree::find_(int idx, int* rp, node** ans)
{
    
    node* p = root;
    while (p->state == State::in_node) // �ҵ�Ҷ�ӽڵ�
    {
        
        int flag = 0;
        int i = 0;
        for (i = 0; i < p->num; i++)
        {
            if (idx <= p->child_idx[i])
            {
                p = p->child[i];
                flag = 1;
                break;
            }
        }
        
        if (!flag)
        {
            p = p->child[p->num - 1];
        }
            
    }
    
    
    if(ans)
    *ans = p;
    int pos = -1;
    for (int i = 0; i < p->num; i++)
    {
        if (idx == p->child_idx[i])
        {
            pos = i;
            if(rp)
            *rp = pos;
            return p->data[pos];
        }
        if (pos == -1 && idx < p->child_idx[i])
            pos = i;
    }
    if (pos == -1)
        pos = p->num;
    if(rp)
    *rp = pos;
    return nullptr;
}
void BAtree::add_(int idx, mydata* data)
{

    //������� //Ϊ�˼򻯴��룬ʹ�ҵĴ��Ժ���⣬�Ұѳ���M���������������Ȼ���ٷ֣�
    int pos;
    node* local_node = new node();
    mydata* tmp = find_(idx, &pos, &local_node);
    for (int i = local_node->num - 1; i >= pos; i--)
    {
        local_node->child_idx[i + 1] = local_node->child_idx[i];
        local_node->data[i + 1] = local_node->data[i];
    }
    
    local_node->child_idx[pos] = idx;
    local_node->data[pos] = data;
    local_node->num++;


    balance_(local_node);
    //cout << "add" << root->state << endl;
    // �ж��Ƿ���Ҫ���򴫲� to be continued;
    update_(local_node);
    
    //cout << "uppdate" << root->state << endl;
}
void BAtree::balance_(node* now_node)
{
    if (now_node == nullptr)return;
    if (now_node->num > M)  // split
    {
        node* new_node = new node();
        new_node->state = now_node->state;

        new_node->num = ceil0;
        now_node->num = ceil0;
        //����Ϊ��num��state�ĸ���
        for (int i = ceil0; i < M + 1; i++)
        {
            new_node->child[i - ceil0] = now_node->child[i];
            new_node->child_idx[i - ceil0] = now_node->child_idx[i];
            new_node->data[i - ceil0] = now_node->data[i];
            fa[new_node->child[i - ceil0]] = new_node;
            fapos[new_node->child[i - ceil0]] = i - ceil0;
            now_node->child[i] = nullptr;
            now_node->child_idx[i] = -1;
            now_node->data[i] = nullptr;
        }
        //����Ϊ��child��child����idx,data�ĸ���
        if (new_node->state == State::leaf_node)
        {
            new_node->bro = now_node->bro;
            now_node->bro = new_node;
        }//����Ϊ��bro�ĸ��£���Ҷ�ӽ��

        node* father = fa[now_node];
        // û�и��ڵ�
        if (father == nullptr)
        {
            node* now_root = new node();
            now_root->num = 2;
            now_root->child_idx[0] = now_node->child_idx[ceil0 - 1];
            now_root->child_idx[1] = new_node->child_idx[ceil0 - 1];
            now_root->child[0] = now_node;
            now_root->child[1] = new_node;
            now_root->state = State::in_node;
            fa[now_root] = nullptr;
            fapos[now_root] = -1;
            root = now_root;
            //cout << root->state<< endl;
            // now_root(father)����Ϣ����

            fa[now_node] = now_root;
            fa[new_node] = now_root;
            fapos[now_node] = 0;
            fapos[new_node] = 1;
            if (!start && now_node->state == State::leaf_node)
                start = now_node;
            // now_node, new_node�ĸ�����Ϣ�ĸ���
            //cout << "outbalance" << root->state << endl;
            //cout << endl << new_node << " " << new_node->state << " " << new_node->child_idx[0] << endl;
            return;
        }
        
        int pos_in_father = fapos[now_node];
        father->child_idx[pos_in_father] = now_node->child_idx[ceil0 - 1];
        for (int i = father->num - 1; i > pos_in_father; i--)
        {
            father->child_idx[i + 1] = father->child_idx[i];
            father->child[i + 1] = father->child[i];
            fapos[father->child[i + 1]]++;
        }
        father->num++;
        father->child_idx[pos_in_father + 1] = new_node->child_idx[ceil0 - 1];
        father->child[pos_in_father + 1] = new_node;
        fa[new_node] = father;
        fapos[new_node] = pos_in_father + 1;

        update_(now_node); update_(new_node);
        
        balance_(fa[now_node]);
        
    }
    else if (now_node->num < ceil0)
    {
        if (now_node == root)
        {
            if (now_node->num >= 2)
                return;
            else if (now_node->num < 2 && now_node->state == State::in_node)
            {
                root = root->child[0];
                
                return;
            }
            else return;
        }
        int pos_in_father = fapos[now_node];
        node* father = fa[now_node];
        int falen = father->num;
        if (pos_in_father + 1 < falen) //�ͺ���ĺϲ� 
        {
            node* old_node = father->child[pos_in_father + 1];
            //���¸ý�㣬���Ϻ���������ݣ�child��childidx data num
            for (int i = 0; i < old_node->num; i++)
            {
                now_node->child[i + now_node->num] = old_node->child[i];
                now_node->child_idx[i + now_node->num] = old_node->child_idx[i];
                now_node->data[i + now_node->num] = old_node->data[i];
                fa[old_node->child[i]] = now_node;
                fapos[old_node->child[i]] += now_node->num;
            }
            now_node->num += old_node->num;
            if (old_node->state == State::leaf_node)
            {
                now_node->bro = old_node->bro;
            }
            for (int i = pos_in_father; i < falen - 1; i++)
            {
                father->child_idx[i] = father->child_idx[i + 1];
                father->child[i] = father->child[i + 1];
                fapos[father->child[i]]--;
            }
            father->child[pos_in_father] = now_node;
            father->num--;

            balance_(now_node);
            update_(now_node);
        }
        else if (pos_in_father - 1 >= 0)                           //��ǰ��ĺϲ�
        {
            node* old_node = father->child[pos_in_father - 1];
            //���¸ý�㣬���Ϻ���������ݣ�child��childidx data num
            for (int i = 0; i < now_node->num; i++)
            {
                old_node->child[i + old_node->num] = now_node->child[i];
                old_node->child_idx[i + old_node->num] = now_node->child_idx[i];
                old_node->data[i + old_node->num] = now_node->data[i];
                fa[now_node->child[i]] = old_node;
                fapos[now_node->child[i]] += old_node->num;
            }
            old_node->num += now_node->num;
            if (old_node->state == State::leaf_node)
            {
                old_node->bro = now_node->bro;
            }
            for (int i = pos_in_father; i < falen - 1; i++)
            {
                father->child_idx[i] = father->child_idx[i + 1];
                father->child[i] = father->child[i + 1];
                fapos[father->child[i]]--;
            }
            father->num--;
            now_node = old_node;
            balance_(now_node);
            update_(now_node);
        }
        balance_(fa[now_node]);
    }
}
void BAtree::update_(node* now_node)
{
    if (now_node == nullptr)return;
    //��ǰ�ڵ��е����ؼ���                       father�ж�Ӧ�ý�㴦�Ĺؼ���
    if (fa[now_node] && now_node->child_idx[now_node->num - 1] != fa[now_node]->child_idx[fapos[now_node]])
    {
        fa[now_node]->child_idx[fapos[now_node]] = now_node->child_idx[now_node->num - 1];
        if (fapos[now_node] == fa[now_node]->num - 1)
        {
            update_(fa[now_node]);
        }
    }
}
void BAtree::delete_(int idx)
{
    int pos;
    node* local_node = new node();
    mydata* tmp = find_(idx, &pos, &local_node);
    if (!tmp)
    {
        cout << idx<<"��㲻���ڣ��޷�ɾ��\n";
        return;
    }
    for (int i = pos; i < local_node->num - 1; i++)
    {
        //local_node->child[i + 1] = local_node->child[i];
        local_node->child_idx[i] = local_node->child_idx[i + 1 ];
        local_node->data[i] = local_node->data[i + 1];
    }
    local_node->num--;
    balance_(local_node);
    update_(local_node);

}
void BAtree::change_(int idx, mydata* data)
{
    int pos = -1;
    node* local_node;
    mydata* tmp = find_(idx, &pos, &local_node);
    local_node->data[pos] = data;
}
vector<mydata*> BAtree::find_interval(int lmin, int rmax)
{
    node* p = start;
    int idx = 0;
    vector<mydata*>ans;
    while (p  && p->child_idx[idx] < lmin)
    {
        if (idx < p->num - 1)
            idx++;
        else
        {
            idx = 0;
            p = p->bro;
        }
    }
    while (p && p->child_idx[idx] <= rmax)
    {
        ans.push_back(p->data[idx]);
        if (idx < p->num - 1)
            idx++;
        else
        {
            idx = 0;
            p = p->bro;
        }
    }
    return ans;
}
void test_output(node* tree, int n)
{
    if (!tree)return;
    for (int i = 0; i < n; i++)
        cout << "\t";
    for (int i = 0; i < tree->num; i++)
    {
        cout << tree->child_idx[i] << "-";
    }
    cout << endl;
    for (int i = 0; i < tree->num; i++)
    {
        test_output(tree->child[i], n + 1);
    }
    cout << endl;
}