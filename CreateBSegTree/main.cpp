//
//  main.cpp
//  multi-branch segment tree
//
//  Created by zph on 16/5/29.
//  Copyright © 2016年 zph. All rights reserved.
//

#include <iostream>
#include <ostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

const int kBranchNum = 4;                   //const branch number
typedef struct Node{                         //Node structure
    int sequence;
    int l;
    int r;
    int branch;
    int maxi,lmaxi,rmaxi,sum;
    int p[kBranchNum+1];
    struct Node *s[kBranchNum+1];
    
} Node;

typedef struct DiskNode{                         //Node structure
    int l;
    int r;
    int branch;
    int maxi,lmaxi,rmaxi,sum;
    int p[kBranchNum+1];
} DiskNode;


typedef struct Kp{                          //pointer structer
    struct Node *s[kBranchNum+1];
} Kp;

int g_data_num = 0;                         //file data number
float * ReadDate(char filename[])
{
    g_data_num = 0;
    float r[20000];
    FILE *fp;
    double result = 0.0;
    char strline[8];                        //each line's max-character
    if((fp = fopen(filename,"r")) == NULL)
    {
        printf("error!");
        return NULL;
    }
    while (!feof(fp))
    {
        fgets(strline,12,fp);               //read a line
        result=atof(strline);
        g_data_num++;
        r[g_data_num] =result;
        
        
    }
    fclose(fp);                             //close file
    printf("read data file successed\n");
    return r;
}

//max value
int max(int a, int b)
{
    return a>b?a:b;
}

//merge branch in CreateTree
void MergeBranchInC(Node *p,int n)
{
    int psum=0,pmaxi=0,plmaxi=0,prmaxi=0;
    for (int i = 1; i < n; i++)
    {
        if (i == 1)
        {
            p->sum = p->s[1]->sum + p->s[2]->sum;
            p->maxi = max(p->s[1]->maxi,max(p->s[2]->maxi,p->s[1]->rmaxi+p->s[2]->lmaxi));
            p->lmaxi = max(p->s[1]->lmaxi,p->s[1]->sum + p->s[2]->lmaxi);
            p->rmaxi = max(p->s[2]->rmaxi,p->s[2]->sum + p->s[1]->rmaxi);
            psum=p->sum ,pmaxi=p->maxi,plmaxi=p->lmaxi,prmaxi=p->rmaxi;
        }
        else if(i > 1)
        {
            int m = i + 1;
            p->sum = psum + p->s[m]->sum;
            p->maxi = max(pmaxi,max(p->s[m]->maxi,prmaxi+p->s[m]->lmaxi));
            p->lmaxi = max(plmaxi,psum + p->s[m]->lmaxi);
            p->rmaxi = max(p->s[m]->rmaxi,p->s[m]->sum + prmaxi);
            psum=p->sum ,pmaxi=p->maxi,plmaxi=p->lmaxi,prmaxi=p->rmaxi;
        }
        
    }
    
}



//merge branch in QuerySeg funcution
//QuerySeg funtion's merge procedure is different from CreateTree funtion merge procedure
Node *MergeBranchInQ(Node *p, Kp *k, Node *q, int n)
{
    Node *res = (Node *)malloc(sizeof(Node));
        //only two branch to merge
    if(n ==1 )
    {
        res->sum = p->sum + q->sum;
        res->lmaxi = max(p->lmaxi,p->sum + q->lmaxi);
        res->rmaxi = max(q->rmaxi,q->sum + p->rmaxi);
        res->maxi = max(p->rmaxi+q->lmaxi,max(p->maxi,q->maxi));
        return res;

    }
    else
    {
        int psum=0,pmaxi=0,plmaxi=0,prmaxi=0;
        for (int i = 1; i < n; i++)
        {
            if (i == 1)
            {
                res->sum = p->sum + k->s[1]->sum;
                res->lmaxi = max(p->lmaxi,p->sum + k->s[1]->lmaxi);
                res->rmaxi = max(k->s[1]->rmaxi,k->s[1]->sum + p->rmaxi);
                res->maxi = max(p->rmaxi+k->s[1]->lmaxi,max(p->maxi,k->s[1]->maxi));
                psum=res->sum ,pmaxi=res->maxi,plmaxi=res->lmaxi,prmaxi=res->rmaxi;
            }
            else if(i > 1)
            {
                res->sum = psum + k->s[i]->sum;
                res->lmaxi = max(plmaxi,psum + k->s[i]->lmaxi);
                res->rmaxi = max(k->s[i]->rmaxi,k->s[i]->sum + prmaxi);
                res->maxi = max(prmaxi+k->s[i]->lmaxi,max(pmaxi,k->s[i]->maxi));
                psum=res->sum ,pmaxi=res->maxi,plmaxi=res->lmaxi,prmaxi=res->rmaxi;
            }
            
        }
        res->sum = psum + q->sum;
        res->lmaxi = max(plmaxi,psum + q->lmaxi);
        res->rmaxi = max(q->rmaxi,q->sum + prmaxi);
        res->maxi = max(prmaxi+q->lmaxi,max(pmaxi,q->maxi));
        return res;
    }
    
}

//create multi-branch tree
void CreateTree(int l ,int r , Node *tp, float x[])
{
    int i = 1;
    int ll = l;
    int rr = r;
    tp->l = l;
    tp->r = r;
    int seg = (r - l) / kBranchNum;   //segment length
    int flag = 0;
    
    if(ll == rr)
    {
        tp->l = ll;
        tp->r = rr;
        tp->maxi = tp->lmaxi = tp->rmaxi = tp->sum = x[ll];
        return;
    }
    
    for (i = 1 ; i < kBranchNum + 1; i++)
    {
        seg = (r - l) / kBranchNum;
        if (ll > r || flag == 1)
        {
            break;
        }
        else if (ll + seg > r && ll < r + 1)
        {
            tp->s[i] = (Node *)malloc(sizeof(Node));  //apply for node
            CreateTree(ll, r,tp->s[i],x);             //create node
            flag = 1;                                 //quit for
        }
        else
        {
            tp->s[i] = (Node *)malloc(sizeof(Node));   //apply for node
            CreateTree(ll, ll + seg,tp->s[i],x);       //create node
            ll = ll + seg + 1;
            rr = ll + seg ;
        }
        tp -> branch = i;
    }
    MergeBranchInC(tp, i - 1);
}

//calculate pointer number
int IntervalNum(int x,Node *p)
{
    int m = 0;
    for(int i = 1;i <= p->branch; i++)
    {
        if (x >= p->s[i]->l && x <= p->s[i]->r) m = i;
    }
    return m;
}

//query segment from aa to bb's maxsub sum
Node *QuerySeg(int l,int r,int aa,int bb, Node *tp,int num)
{
    int flag1 = 0;                        //only one branch
    int flag2 = 0;                        //if should merge branch
    if(num != 0) tp = tp -> s[num];       //change pointer
    Node *ka, *kl, *kr, *res, *res1;
    Kp *k = (Kp *)malloc(sizeof(Kp));     //multi-branch's pointer
    res = (Node *)malloc(sizeof(Node));
    ka = (Node *)malloc(sizeof(Node));
    kl = (Node *)malloc(sizeof(Node));
    kr = (Node *)malloc(sizeof(Node));
    if(aa <= l && bb >= r)
        return tp;
    int ll = 0;
    int rr = 0;
    ll = IntervalNum(aa, tp);             //calculate pointer number
    rr = IntervalNum(bb, tp);
    if(ll == rr)
    {
        if(tp -> s[rr] -> r < bb)
            ka = QuerySeg(tp -> s[ll] ->r,tp -> s[rr]->r, aa, tp -> s[rr]->r, tp, ll);
        else
            ka = QuerySeg(tp -> s[ll] ->l,tp -> s[rr]->r, aa, bb, tp, ll);
        flag1 = 1;
    }
    int i = 1;
    if(ll < rr)
    {
        kl = QuerySeg(tp -> s[ll] ->l,tp -> s[ll] ->r, aa, tp -> s[ll] ->r, tp, ll);//lefmost point
        while (ll < rr - 1)
        {
            k -> s[i] = QuerySeg(tp -> s[ll+1] ->l,tp -> s[ll+1] ->r, tp -> s[ll+1] ->l, tp -> s[ll+1] ->r, tp, ll+1);
            ll++;
            i++;
        }
        kr = QuerySeg(tp -> s[rr] ->l,tp -> s[rr] ->r, tp -> s[rr] ->l, bb, tp, rr);//rightmost point
        flag2 = 1;
        
    }
    if (flag1 == 1)
    {
        res = ka;
    }
    else if(flag2 == 1)
    {
        res1 = MergeBranchInQ(kl, k, kr, i);
        res -> sum = res1 -> sum;
        res -> lmaxi = res1 -> lmaxi;
        res -> rmaxi = res1 -> rmaxi;
        res -> maxi = res1 -> maxi;
    }
    return res;
}

////level traversal
//void LevelTra(Node* root)
//{
//    if (!root)
//    {
//        return;
//    }
//    Node *indexnode;
//    int nodeszie = sizeof(DiskNode);
//    ofstream outfile("index.dat",ios::out|ios::binary);  //create index file
//    if(!outfile)
//        cout << "error! can't create file" << endl;
//    //outfile.write((char*)indexnode, sizeof(Node)*10);
//    vector<Node*> vec;
//    vec.push_back(root);
//    int cur=0;
//    int last=1;
//    Node *p = root;
//    while (cur<vec.size())
//    {
//        last=int(vec.size());
//        while (cur<last)
//        {
//            cout<<vec[cur]->l<<"  ";
//            outfile.seekp(nodeszie*cur,ios::beg);
//            char writendoe = vec[cur]->l+vec[cur]->r+vec[cur]->branch+vec[cur]->maxi+vec[cur]->lmaxi+vec[cur]->rmaxi+vec[cur]->sum;
//            outfile.write((char*)writendoe, sizeof(DiskNode));
//            outfile.write();
//            for(int i = 1;i <= p->branch; i++)
//            {
//                vec.push_back(vec[cur]->s[i]);
//                //p = vec[cur];
//            }
//            cur++;
//            p = vec[cur];
//        }
//        cout<<endl;
//    }
//}

void LevelTra(Node* root)
{
    if (!root)
    {
        return;
    }
    vector<Node*> vec;
    vec.push_back(root);
    
    int cur=0;
    int last=1;
    Node *p = root;
    while (cur<vec.size())
    {
        last=int(vec.size());
        while (cur<last)
        {
            cout<<vec[cur]->l<<"  ";
            vec[cur] -> sequence = cur;
            for(int i = 1;i <= p->branch; i++)
            {
                vec.push_back(vec[cur]->s[i]);
                //p = vec[cur];
            }
            cur++;
            p = vec[cur];
        }
        cout<<endl;
    }
    
    cur = 0;
    last = 1;
    p =root;
    while (cur<vec.size())
    {
        last=int(vec.size());
        while (cur<last)
        {
            for(int i = 1;i <= p->branch; i++)
            {
                vec[cur] -> p[i] = vec[cur] -> s[i] -> sequence * sizeof(Node);
                vec.push_back(vec[cur]->s[i]);
            }
            cur++;
            p = vec[cur];
        }
        cout<<endl;
    }
}


void WriteIndexFile(Node *rootnode)
{
    fstream file;
    file.open("test.dat", ios::out | ios::trunc | ios::binary);  //create index file
    if(!file)
        cout << "error! can't create file" << endl;
    
    DiskNode *disknode =(DiskNode *)malloc(sizeof(DiskNode));
    vector<Node*> vec;
    vec.push_back(rootnode);
    
    int cur=0;
    int last=1;
    int nodesize = sizeof(Node);
    Node *p = rootnode;
    while (cur<vec.size())
    {
        last=int(vec.size());
        while (cur<last)
        {
            cout<<vec[cur]->l<<"  ";
            disknode =(DiskNode *)malloc(sizeof(DiskNode));
            disknode -> l = vec[cur] -> l;
            disknode -> r = vec[cur] -> r;
            disknode -> branch = vec[cur] -> branch;
            disknode -> maxi = vec[cur] -> maxi;
            disknode -> lmaxi = vec[cur] -> lmaxi;
            disknode -> rmaxi = vec[cur] -> rmaxi;
            disknode -> sum = vec[cur] -> sum;
            //disknode -> l = vec[cur] -> l;
            //disknode -> l = vec[cur] -> l;
            //disknode -> l = vec[cur] -> l;
            //Node *disknode = vec[cur];
            //outfile.seekp(sizeof(DiskNode)*cur);
            //file.seekp(sizeof(DiskNode)*cur, ios::beg);
            file.write((char*)&disknode, sizeof(DiskNode));
            file.seekp(0, ios::end);
            free(disknode);
            for(int i = 1;i <= p->branch; i++)
            {
                vec.push_back(vec[cur]->s[i]);
            }
            cur++;
            p = vec[cur];
        }
        cout<<endl;
    }
    file.seekp(0,ios::end);
    file.close();
}

Node *rootnode = (Node *)malloc(sizeof(Node)); //apply for root node
int main(int argc, const char * argv[]) {
    float *p;
    p = ReadDate("/Users/zph/XcodeProject/编程珠玑/SegTreeMaxSub/SegTreeMaxSub/100.txt");
    printf("total data is %d\n",g_data_num);
    CreateTree(1, 16, rootnode, p);
    LevelTra(rootnode);
    WriteIndexFile(rootnode);
    Node *res = QuerySeg(1, 3, 1, 3, rootnode, 0);
    printf("maxsub sum is :%d\n",res->maxi);
    free(rootnode);
    return 0;
}
