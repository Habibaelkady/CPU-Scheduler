#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;

struct Process{
    char pid;
    int id;
    int tarrival;
    int tservice;
    int tstart;
    int tfinish;
    int tturnaround;
    int tready;
    int twaiting=0;
    int tchecking;
    int tremaining ;
    int tworking ;
    int priority;
    int finished;
    float normTurnaround;
    int quantum;
    float responseRatio;
};

struct CompareServTime {
    bool operator()(Process const& p1, Process const& p2)
    {
        return p1.tservice > p2.tservice;
    }
};
struct ComparePriorities {
    bool operator()(Process const& p1, Process const& p2)
    {
        return p1.priority > p2.priority;
    }
};
struct CompareRemTime {
    bool operator()(Process const& p1, Process const& p2)
    {
        return p1.tremaining > p2.tremaining;
    }
};
struct CompareArrTime {
    bool operator()(Process const& p1, Process const& p2)
    {
        return p1.tarrival > p2.tarrival;
    }
};

bool cmpPriority(Process p1, Process p2)
{
    if(p1.priority!=p2.priority)
        return p1.priority > p2.priority;
    else
        return p1.twaiting > p2.twaiting;
}

// struct CompareArrTime {
//     bool operator()(Process const& p1, Process const& p2)
//     {
//         return p1.tservice > p2.tservice;
//     }
// };
bool cmpArrivalTime(Process a, Process b)
{
    return a.tarrival < b.tarrival;
}


void printStats(Process p[],int np){
    cout<<"Process    |";
    for(int i = 0;i<np;i++){
        cout<<"  "<<p[i].pid<<"  |";
    }
    cout<<"\n";
    cout<<"Arrival    |";
    for(int i = 0;i<np;i++){
        if((p[i].tarrival/10)==0)
        cout<<"  "<<p[i].tarrival<<"  |";
        else
        cout<<" "<<p[i].tarrival<<"  |";
    }
    cout<<"\n";
    cout<<"Service    |";
    for(int i = 0;i<np;i++){
        if((p[i].tservice/10)==0)
        cout<<"  "<<p[i].tservice<<"  |";
        else
        cout<<" "<<p[i].tservice<<"  |";

    }
    cout<<" Mean|";
    cout<<"\n";
    cout<<"Finish     |";
    for(int i = 0;i<np;i++){
        if((p[i].tfinish/10)==0)
        cout<<"  "<<p[i].tfinish<<"  |";
        else
        cout<<" "<<p[i].tfinish<<"  |";
    }
    cout<<"-----|";
    cout<<"\n";
    cout<<"Turnaround |";
    int sum=0;
    for(int i = 0;i<np;i++){
        if((p[i].tturnaround/10)==0)
        cout<<"  "<<p[i].tturnaround<<"  |";
        else
        cout<<" "<<p[i].tturnaround<<"  |";
        sum+=p[i].tturnaround;
        
    }
    float avg=sum*1.0/np;
    if((int(avg)/10))
    printf("%.2f|",avg); //two digits before decimal place
    else
    printf(" %.2f|",avg); //one digit before decimal place
    cout<<"\n";
    cout<<"NormTurn   |";
    float summ=0;
    for(int i = 0;i<np;i++){
        printf(" %.2f|",p[i].normTurnaround);
        summ+=p[i].normTurnaround;
    }
    float avgg=summ*1.0/np;
    if((int(avgg)/10))
    printf("%.2f|",avgg); //two digits before decimal place
    else
    printf(" %.2f|",avgg); //one digit before decimal place

    cout<<"\n";
    cout<<"\n";

}

void printTrace(Process p[], int np, char arr[][20] ){
     for(int i=0 ; i<np ; i++)
    {   
        cout<<p[i].pid<<"     ";
        cout<<"|";
        for(int j = 0 ; j<20 ; j++)
            cout<<arr[i][j]<<"|";
        cout<<" "<<"\n";
    }
}

void fcfsStats(Process p[], int np,char arr[][20]){
    p[0].tstart = p[0].tarrival;
    p[0].tfinish = p[0].tstart + p[0].tservice;
    p[0].tturnaround = p[0].tfinish - p[0].tarrival;
    p[0].normTurnaround = p[0].tturnaround/p[0].tservice;
    p[0].twaiting = 0;
    p[0].tremaining=0;
    p[0].tchecking=p[0].tfinish;
    p[0].tworking=p[0].tservice;
    p[0].priority=0;
    p[0].id=0;
    for(int i = 1;i<np;i++){
        p[i].tstart = p[i-1].tfinish;
        p[i].tfinish = p[i].tstart + p[i].tservice;
        p[i].tturnaround = p[i].tfinish - p[i].tarrival;
        p[i].normTurnaround = p[i].tturnaround*1.0/p[i].tservice;
        p[i].twaiting = p[i].tstart - p[i].tarrival;
        p[i].tremaining=0;
        p[i].tchecking=p[i].tfinish;
        p[i].tworking=p[i].tservice;
        p[i].priority=0;
        p[i].id=i;
    }
    for(int k=0;k<np;k++){

    for(int j = p[k].tstart ; j<p[k].tfinish;j++){
            arr[k][j]='*';
        }
    for(int j=p[k].tarrival;j<p[k].tstart;j++){
            arr[k][j]='.';
        }
    }
   }
   
   
void rrStats(Process p[],int np,int q,char arr[ ][20]){
    int trunning = 0; //running time of round robin
    int finishedProcesses = 0; //keeps track of the number of processes that finishe execution
    int i;
    deque<int> qq;
    qq.push_back(0); //first process
    for (int i= 0 ; i<np ; i++){
        p[i].tworking=0;
        p[i].twaiting=0;
        p[i].id = i;
        p[i].tready = 0;
        p[i].tremaining=p[i].tservice;
        p[i].priority=0;
    }
    while(finishedProcesses!=np){
        i = qq.front();
        qq.pop_front();
        //process entering for the first time
        if(p[i].tremaining == p[i].tservice){
            if(trunning>p[i].tarrival){ 
                p[i].tstart = trunning;
                //the difference between current time and arrival time is the time the process spent waiting
                for(int j =0;j<=trunning-p[i].tarrival;j++){
                    arr[p[i].id][trunning-j] = '.';
                }
            }
            else p[i].tstart = p[i].tarrival;
            arr[p[i].id][trunning] = '*';
        }
        
        //process will finish executing in the current quantum
        if (p[i].tremaining<=q && p[i].tremaining>0){
            if(p[i].tready){
                p[i].twaiting = trunning - p[i].tready;
                //track down the time the process spent waiting before getting admitted 
                for(int j =1;j<=p[i].twaiting;j++){
                arr[p[i].id][trunning-j] = '.';
                }
            }

            //process will be running for its remaining time (the remaining time is less than or equal to q)
            for(int j =0;j<p[i].tremaining;j++){
                arr[p[i].id][trunning+j] = '*';
            }
            
            //update current time after process finished
            trunning += p[i].tremaining;

            //push all elements with arrival time less than or equal current time that are not present in the queue and that have remaining time greater than zero
            for(int j=1;j<np-i;j++){
                deque<int>::iterator it = find(qq.begin(),qq.end(),i+j);
                if(it!=qq.end()){}
                else{
                    if(p[i+j].tarrival<=trunning && p[i+j].tremaining>0)
                    qq.push_back(i+j);   
                }
                }  

            p[i].tremaining = 0;
            p[i].tfinish = trunning;
            p[i].tturnaround = p[i].tfinish -p[i].tarrival;
            p[i].normTurnaround = p[i].tturnaround*1.0/p[i].tservice;
            finishedProcesses++;
        }
        else{
            if(p[i].tready){
                p[i].twaiting = trunning - p[i].tready;
                for(int j =1;j<=p[i].twaiting;j++){
                arr[p[i].id][trunning-j] = '.';
                }
            }
            for(int j =0;j<q;j++){
                arr[p[i].id][trunning+j] = '*';
            }
            trunning+=q;
            for(int j=1;j<np-i;j++){
                deque<int>::iterator it = find(qq.begin(),qq.end(),i+j);
                if(it!=qq.end()){}
                else{
                    if(p[i+j].tarrival<=trunning && p[i+j].tremaining>0)
                    qq.push_back(i+j);   
                }
                }  
            p[i].tremaining -=q;
            p[i].tready = trunning;
            qq.push_back(i);
        }
        if(qq.empty()) {
            for(int i = 1; i < np; i++) {
                if(p[i].tremaining > 0) {
                    p[i].tready = trunning;
                    qq.push_back(i);
                    break;
                }
            }
        }   
    }        
}
void spnStats(Process p[], int np, char arr [][20]){
    int timeNow=0;
    queue<Process> q;
    Process temp;
    priority_queue<Process , vector<Process>,CompareServTime>pq;
    for (int i= 0 ; i<np ; i++){
        p[i].id=i;
        p[i].twaiting=0;
        p[i].tworking=0;
        p[i].tremaining=p[i].tservice;
        p[i].tchecking=0;
        p[i].priority=0;
        p[i].quantum=1;
        q.push(p[i]);

    }
    int j=0;
    while(!q.empty()||!pq.empty()){
        while(!q.empty()){
            if(q.front().tarrival <= timeNow)
            {
                pq.push(q.front()); //ready queue "the process with least service time runs first"
                q.pop();
            }
            else
            break;

        }
        if(!pq.empty()){
        temp = pq.top(); 
        temp.tstart=timeNow;
        temp.tfinish=temp.tstart + temp.tservice;
        timeNow=temp.tfinish;
        temp.tturnaround = temp.tfinish - temp.tarrival;
        temp.normTurnaround = temp.tturnaround *1.0 / temp.tservice;
        temp.twaiting=temp.tarrival-temp.tstart;
        temp.tchecking=temp.tfinish;
        temp.tworking=temp.tservice;
        for(int i=temp.tstart ; i<temp.tfinish;i++)
        arr[temp.id][i] = '*';
        for(int i=temp.tarrival ; i<temp.tstart;i++)
        arr[temp.id][i] = '.';
        p[j]=temp;
        j=j+1;
        pq.pop();
        }
        else
        timeNow+=1;        //there is no process in the ready queue to start running then only increase the current time 
    }
    sort(p,p+np,cmpArrivalTime);// sorting the processes by arrival time again to be printed correctly 

}
void srtStats(Process p[], int np,char arr[][20]){
        int timeNow=0;
       // int id = 0;
    queue<Process> q; //queue containing processes available on processor on the current time
    priority_queue<Process , vector<Process>,CompareRemTime>pq; // queue containing processes sorted according to shortest remaining time 
    for (int i= 0 ; i<np ; i++){
        p[i].tworking=0;
        p[i].twaiting=0;
        p[i].id = i;
        p[i].tremaining=p[i].tservice;
        p[i].priority=0;
        p[i].quantum=1;
        q.push(p[i]);
        //id+=1;

    }
    int j=0;
    Process temp;
    while(!q.empty()||!pq.empty()){
        while(!q.empty()){
            if(q.front().tarrival <= timeNow)
            {
                pq.push(q.front());
                q.pop();
            }
            else
            break;

        }
        if(pq.size()>=2){ // to make sure if 2 processes have the same remaining time thst the first one "with least arrival time" works first
        Process last = pq.top();
        pq.pop();
        Process plast=pq.top();
        pq.pop();
        
        if(last.tremaining == plast.tremaining)
        {   if(last.tarrival<= plast.tarrival ){
            pq.push(last);
            pq.push(plast);
            }
            else 
            {
                pq.push(plast);
                pq.push(last);
            }
        }  
        else{
            pq.push(plast);
            pq.push(last);
        }
        }
        temp = pq.top();
        pq.pop();
        if(temp.tworking==0){
        temp.tstart=timeNow;
        temp.tchecking = temp.tstart; // to check the last time the process worked 
        }
        temp.tworking +=1;
        arr [temp.id][timeNow] = '*';
        timeNow+=1;       
        temp.tchecking+=1;
        if(timeNow!= temp.tchecking ){ // to calculate the waiting time of a process 
            temp.twaiting += (timeNow - temp.tchecking); 
            temp.tchecking=timeNow;
        }
        temp.quantum=temp.tchecking;
        temp.tremaining=temp.tservice - temp.tworking;
        
        if(temp.tremaining==0){
            pq.push(temp);
            temp.tfinish=temp.tstart + temp.twaiting + temp.tservice ;
            temp.tturnaround = temp.tfinish - temp.tarrival;
            temp.normTurnaround = temp.tturnaround *1.0 / temp.tservice;
            //cout<<temp.tworking<<"working time of process"<<temp.pid<<endl;
            for(int i=temp.tarrival ; i<temp.tfinish ;i++)
            {      
                if(arr[temp.id][i] != '*') // if the process has entered the ready queue but not working 
                arr[temp.id][i] ='.';  // assign its value to '.' which indicates waiting 
            }
            p[j]=temp;
            j=j+1;
            pq.pop();
        }
        else
        pq.push(temp);
        
    }
    sort(p,p+np,cmpArrivalTime);

   
}

void hrrnStats(Process p[], int np,int end,char arr[ ][20]){
    int trunning=0;
    for (int i= 0 ; i<np ; i++){
        p[i].id=i;
        p[i].finished = 0;
        p[i].responseRatio = 0;
    }
    float maxRatio;
    int chosen;
    trunning = p[0].tarrival;
    while(trunning<end){
        maxRatio = INT16_MIN;
        for(int i=0;i<np;i++){
            if(p[i].tarrival<=trunning && p[i].finished==0){
                int wait = trunning - p[i].tarrival;
                p[i].responseRatio = (1.0*(wait + p[i].tservice)) /p[i].tservice;
                for(int j =1;j<=wait;j++)
                    arr[p[i].id][trunning-j] = '.';
                maxRatio = max(maxRatio,p[i].responseRatio);
                if(p[i].responseRatio==maxRatio)
                    chosen = i;
            }
        }
        
        for(int i=0;i<p[chosen].tservice;i++){
            arr[p[chosen].id][trunning+i] = '*';
        }
        trunning += p[chosen].tservice;
        p[chosen].finished=1;
        p[chosen].tfinish = trunning;
        p[chosen].tturnaround = p[chosen].tfinish -p[chosen].tarrival;
        p[chosen].normTurnaround = (1.0*p[chosen].tturnaround)/p[chosen].tservice; 
    }
    

}
void fbStats(Process p[], int np,char arr[ ][20]){
    // initArray(np,arr);
    int timeNow=0;
    queue<Process> q; //queue containing processes available on processor on the current time
    priority_queue<Process,vector<Process> ,ComparePriorities > nextP;// queue containing processes sorted according to shortest remaining time 
    priority_queue<Process,vector<Process>,CompareArrTime> pq;
    for (int i= 0 ; i<np ; i++){
        p[i].tworking=0;
        p[i].twaiting=0;
        p[i].tremaining=p[i].tservice;
        p[i].id=i;
        p[i].priority=0;
        p[i].quantum=1;
        q.push(p[i]);

    }

    int j=0;
    Process temp,t;
    while(!q.empty()||!nextP.empty()){
        while(!q.empty()){
            if(q.front().tarrival <= timeNow)
            {  
                nextP.push(q.front());
                q.pop();
            }
            else
            break;

        }
        while(!nextP.empty()){
            pq.push(nextP.top());
            nextP.pop();
        } //arrange them according to arrival time so that if two have the same priority the one that came first works first
        while (!pq.empty())
        {
            nextP.push(pq.top());
            pq.pop();
        }
        

        temp = nextP.top();
        nextP.pop();
        if(temp.tworking==0){
        temp.tstart=timeNow; //indicates the real time the process starts working
        temp.tchecking = temp.tstart; 
        }
        temp.tworking +=1;
        arr [temp.id][timeNow] = '*'; //indicates that the process has been working in this time interval
        timeNow +=1;       
        temp.tchecking +=1; 
        if(timeNow!= temp.tchecking ){ //checks if the current process has been preemptied before
            temp.twaiting += (timeNow - temp.tchecking); 
            temp.tchecking=timeNow;
        }
        temp.tremaining=temp.tservice - temp.tworking;
        if(temp.tremaining==0){
            temp.tfinish=temp.tstart + temp.twaiting + temp.tservice ;
            temp.tturnaround = temp.tfinish - temp.tarrival;
            temp.normTurnaround = temp.tturnaround *1.0 / temp.tservice;
            for(int i=temp.tarrival ; i<temp.tfinish ;i++)
            {      
                if(arr[temp.id][i] != '*') // if the process has entered the ready queue but not working 
                arr[temp.id][i] ='.';  // assign its value to '.' which indicates waiting 
            }
            p[j]=temp;
            j=j+1;
           
        }
        else
        {
            while(!q.empty()){
            if(q.front().tarrival <= timeNow)
            {
                nextP.push(q.front());
                q.pop();
            }
            else
            break;

            }
        if(!nextP.empty())//check if there is an existing process to decrease the last working process priority  
        temp.priority+=1;

        nextP.push(temp);
        }
      }
    sort(p,p+np,cmpArrivalTime);
    }
void fb2iStats(Process p[], int np,char arr[ ][20]){
    // initArray(np,arr);
    int timeNow=0;
    queue<Process> q; //queue containing processes available on processor on the current time
    priority_queue<Process,vector<Process> ,ComparePriorities > nextP;// queue containing processes sorted according to shortest remaining time 
    priority_queue<Process,vector<Process>,CompareArrTime> pq;
    for (int i= 0 ; i<np ; i++){
        p[i].tworking=0;
        p[i].twaiting=0;
        p[i].tremaining=p[i].tservice;
        p[i].id=i;
        p[i].priority=0;
        p[i].quantum=1;
        q.push(p[i]);

    }

    int j=0;
    Process temp,t;
    while(!q.empty()||!nextP.empty()){
        while(!q.empty()){
            if(q.front().tarrival <= timeNow)
            {  
                nextP.push(q.front());
                q.pop();
            }
            else
            break;

        }
        while(!nextP.empty()){
            pq.push(nextP.top());
            nextP.pop();
        } //arrange them according to arrival time so that if two have the same priority the one that came first works first
        while (!pq.empty())
        {
            nextP.push(pq.top());
            pq.pop();
        }
        

        temp = nextP.top();
        nextP.pop();
        if(temp.tworking==0){
        temp.tstart=timeNow; //indicates the real time the process starts working
        temp.tchecking = temp.tstart; 
        }

        if(temp.quantum>=temp.tremaining)
        temp.quantum=temp.tremaining;
       
        temp.tworking +=temp.quantum;
        for(int i=timeNow;i<timeNow+temp.quantum;i++)
        arr [temp.id][i] = '*'; //indicates that the process has been working in this time interval
        timeNow +=temp.quantum;       
        temp.tchecking +=temp.quantum; 
        if(timeNow!= temp.tchecking ){ //checks if the current process has been preemptied before
            temp.twaiting += (timeNow - temp.tchecking); 
            temp.tchecking=timeNow;
        }
        temp.tremaining=temp.tservice - temp.tworking;
        if(temp.tremaining==0){
            temp.tfinish=temp.tstart + temp.twaiting + temp.tservice ;
            temp.tturnaround = temp.tfinish - temp.tarrival;
            temp.normTurnaround = temp.tturnaround *1.0 / temp.tservice;
            for(int i=temp.tarrival ; i<temp.tfinish ;i++)
            {      
                if(arr[temp.id][i] != '*') // if the process has entered the ready queue but not working 
                arr[temp.id][i] ='.';  // assign its value to '.' which indicates waiting 
            }
            p[j]=temp;
            j=j+1;
           
        }
        else
        {
            while(!q.empty()){
            if(q.front().tarrival <= timeNow)
            {
                nextP.push(q.front());
                q.pop();
            }
            else
            break;

            }
        if(!nextP.empty())//check if there is an existing process to decrease the last working process priority  
        {temp.priority+=1;
        temp.quantum*=2; //increase the next time to work[quantum] according to priority
        }

        nextP.push(temp);
        }
      }
    sort(p,p+np,cmpArrivalTime);
    }


queue<Process> sortQueue(queue<Process> &q)
{
   vector<Process> temp;
   while(!q.empty())
   {
       temp.push_back(q.front());
       q.pop();
   }
   sort(temp.begin(),temp.end(),cmpPriority);
   for(int i=0;i<temp.size();i++)
   {
       q.push(temp[i]);
   }
   return q;
}
queue<Process> incQueue(queue<Process> &q,int quantum)
{
   vector<Process> temp;
   while(!q.empty())
   {
       Process t = q.front();
       t.priority++;
       t.twaiting+=quantum;
       temp.push_back(t);
       q.pop();
   }
   
   for(int i=0;i<temp.size();i++)
   {
       q.push(temp[i]);
   }
   return q;
}



void agingTrace(Process p[], int np,int q,int end,char arr[ ][20]){
    //tservice is treated like a priority here
    int trunning = p[0].tarrival;
    int admitted[np] = {0};
  
    queue<Process> ready;
    queue<Process>copy;
    for(int i=0; i<np;i++){
        p[i].priority = p[i].tservice;
        p[i].id = i;
        p[i].tready = 0;
        p[i].twaiting = 0;
    }
    ready.push(p[0]);
    //checks if a process is admitted for the first time in the ready queue
    admitted[0]=1;
    
    //ready = sortQueue(ready);
    while(trunning<end){
        for(int i=0;i<np;i++){
            if(p[i].tarrival<=trunning && admitted[i]!=1){
                //p[i].tready = trunning-p[i].tarrival;
                p[i].twaiting = trunning-p[i].tarrival;
                p[i].priority++;
                ready.push(p[i]);
                admitted[i]=1; 
                
            }
        }
        //sort ready queue according to priority and admit the largest priority process to the proccessor
        ready = sortQueue(ready);
        Process temp = ready.front();
        ready.pop();
        //reset the priority of the current process
        temp.priority = temp.tservice;
        //reset the waiting time of the current process
        temp.twaiting=0;
        
        //print . for all other processes for the next quantum since they will be waiting
        copy = ready;
        for(int i=0;i<q;i++){
            while(!copy.empty()){
            Process t = copy.front();
            copy.pop();
            arr[t.id][trunning+i] = '.';
        }
        }
        // print * for the current process in the current quantum
        for(int i = 0;i<q;i++){
            arr[temp.id][trunning+i] = '*';
        }

        //increase priority of all other processes
        ready = incQueue(ready,q);
        //return current process to ready queue
        ready.push(temp);
        trunning +=q;
    }

    
}
void initArray(int np,char arr[][20]){
    for(int i=0 ; i<np ; i++)
    {
        for(int j = 0 ; j<20 ; j++)
        arr[i][j]=' ';
    }

}
int main(){
    string pol;
    char mode [10];
    int end,np;
    vector<string> policies;
    char del = ',';
    bool stats=0;
    cin>>mode;
    cin>>pol;
    cin>>end;
    cin>>np;
    Process p[np];
    char trace[np][20];
    

    if (strcmp("stats",mode)==0)
    stats=1;
    
    for(int i=0;i<np;i++){
        cin>>p[i].pid>>del>>p[i].tarrival>>del>>p[i].tservice;
    }
    sort(p,p+np,cmpArrivalTime);
    stringstream ss(pol);
    while(ss.good())
    {
        string substr;
        getline(ss, substr, del);
        policies.push_back(substr);
    }
    if(stats){
        for(int i = 0; i<policies.size();i++){
        if(policies[i].size()==1){
        switch(policies[i][0]){
            case '1':
                cout<<"FCFS"<<"\n";
                fcfsStats(p,np,trace);
                printStats(p,np);
                break;
            case '3':
                spnStats(p,np,trace);
                cout<<"SPN"<<"\n";
                printStats(p,np);
                break;
            case '4':
                srtStats(p,np,trace);
                cout<<"SRT"<<"\n";
                printStats(p,np);
                break;
            case '5':
                hrrnStats(p,np,end,trace);
                cout<<"HRRN"<<"\n";
                printStats(p,np);
                break;
            case '6':
                fbStats(p,np,trace);
                cout<<"FB-1"<<"\n";
                printStats(p,np);
                break;
            case '7':
                cout<<"FB-2i"<<"\n";
                fb2iStats(p,np,trace);
                printStats(p,np);
                break;
            default:
                break;
        }
        }
        else{
            if(policies[i][0]=='2'){
                int q = int(policies[i][2])-'0';
                //cout<<q<<endl;
                rrStats(p,np,q,trace);
                cout<<"RR-"<<policies[i][2]<<"\n";
                printStats(p,np);
            }
        }
    }
    }
    else {

    for(int i = 0; i<policies.size();i++){
            initArray(np,trace);
            if(policies[i].size()==1){
            switch(policies[i][0]){
                case '1':
                    cout<<"FCFS  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    fcfsStats(p,np,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                    break;
                case '3':
                    cout<<"SPN   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    spnStats(p,np,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                    break;
                case '4':
                    cout<<"SRT   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    srtStats(p,np,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                    break;
                case '5':
                    cout<<"HRRN  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    hrrnStats(p,np,end,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                    break;
                case '6':
                    cout<<"FB-1  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    fbStats(p,np,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                    break;
                case '7':
                    cout<<"FB-2i 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    fb2iStats(p,np,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                    break;
                default:
                    break;
            }
            }
            else{
                if(policies[i][0]=='2'){
                    cout<<"RR-"<<policies[i][2]<<"  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    int q = int(policies[i][2])-'0';
                    rrStats(p,np,q,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                }
                else{
                    cout<<"Aging "<<"0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "<<"\n";
                    cout<<"------------------------------------------------"<<"\n";
                    int q = int(policies[i][2])-'0';
                    agingTrace(p,np,q,end,trace);
                    printTrace(p,np,trace);
                    cout<<"------------------------------------------------"<<"\n";
                    cout<<"\n";
                }
            }
        }
    }




    return 0;
}