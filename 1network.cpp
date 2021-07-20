#include<bits/stdc++.h>
#include<iomanip>
using namespace std;
#define INF 999999999

map <string, int> mp;
map <int , string> rev_mp;

//int label = 0;
int n;

class network {
	vector <vector <pair <int , int> > > v;
public:
	network() {
		v.resize(n+1);
	}
	void create(string id , int wt);
	vector <int> djikstras(int src);
	void create_final(network tcp , network udp , network icmp, network any);
	void display();
};


void network :: create(string id, int wt) {
	//cout << "inside create";
	int count = 1;
	string line , ip , status ,proto,word;
	
	while(count <= n) {
		set <string> check_csv;
		check_csv.clear();
		fstream fd;
		string s = to_string(count) +  ".csv";
		fd.open(s , ios :: in);
		getline(fd , line);
		//while(fd.good()) {
			while(getline(fd , line))
			{
				stringstream s(line); 
				getline(s , ip, ','); 
				check_csv.insert(ip);
			}
		//}
		fd.close();
		fd.open(s , ios :: in);
		vector<string> row; 
		string temp;
		getline(fd , line);
		//while(fd.good()) {
			while(getline(fd , line)) {
				row.clear();  
        		stringstream s(line); 
				while (getline(s, word, ',')) { 
		            row.push_back(word); 
		        } 

				proto = row[1];
				status = row[2];

				int curr_ip = mp[row[0]]; 
			
				int t1,t2;
				t1 = id.compare(proto);
				t2 = status.compare("ALLOW");
				
				
				if(proto == "all" && status == "ALLOW" && row[0] != "ANY" && curr_ip != count) {
					v[count].push_back(make_pair(curr_ip , wt));	
				}
				else if(proto == "all" && status == "ALLOW" && row[0] == "ANY" && curr_ip != count) {
					for(auto it : mp) {
						if(check_csv.find(it.first) == check_csv.end() && mp[it.first] != count) {
							v[count].push_back(make_pair(mp[it.first] , wt));	
						}
					}				
				}
				else if((t1==0) && (t2==0)) {
					if(row[0]=="ANY")
					{			
						for(auto it : mp) {
							if(check_csv.find(it.first) == check_csv.end() && mp[it.first] != count) {
								v[count].push_back(make_pair(mp[it.first] , wt));	
							}
						}	
					}	
					else if(row[0]!="-" && curr_ip != count)
					{
						v[count].push_back(make_pair(curr_ip , wt));	
					}	
				}
			}
			fd.close();
		//}
		count++;
	}
}





vector<int> network :: djikstras(int source) {
	vector<int> dist(n+1, INF);
	set< pair<int, int> > queue;
	vector<bool> visited(n+1, false);
	dist[source] = 0;
	visited[source] = true;
	queue.insert(make_pair(dist[source], source));

	while(!queue.empty()) {
		pair<int, int> front_p = *(queue.begin());
		queue.erase(queue.begin());
		int cur_dist = front_p.first;
		int node = front_p.second;

		for(int i = 0 ; i < v[node].size(); i++) {
			int to = v[node][i].first;
			int weight = v[node][i].second;
			if(dist[to] > cur_dist + weight) {
				if(queue.find(make_pair(dist[to], to)) != queue.end()) {
					queue.erase(make_pair(dist[to], to));
				}
				dist[to] = cur_dist + weight;
				queue.insert(make_pair(dist[to], to));
			}
		}	
	}
	return dist;
}




void network :: display() {
	cout << setw(8) << "Source" << setw(18) << "Destinations" << endl;
	for(int i = 1 ; i <= n ; i++) {
		cout << setw(7) << i << "  ->    ";
		for(int j = 0 ; j < v[i].size() ; j++) {
			cout << "(" << v[i][j].first << " , " << v[i][j].second << ")   ";
		}
		cout << "\n";
	}
}



void network::create_final(network tcp , network udp , network icmp , network any)
{
	for(int i=1; i<=n; i++)
	{
		bool f1 = false , f2 = false, f4 = false ;
		for(int j = 0 ; j < tcp.v[i].size() ; j++)
		{
			int temp = tcp.v[i][j].second;
				v[tcp.v[i][j].first].push_back(make_pair(i,temp));
		}

		for(int j = 0 ; j < udp.v[i].size() ; j++)
		{
			f1 = false;
			int temp = udp.v[i][j].second;
			for(int k = 0 ; k < v[udp.v[i][j].first].size() ; k++)
			{
				if(v[udp.v[i][j].first][k].first == i)
				{
					temp += v[udp.v[i][j].first][k].second;
					v[udp.v[i][j].first][k].second = temp;
					f1 = true;
					break;
				}
			}
			if(!f1)
				v[udp.v[i][j].first].push_back(make_pair(i,temp));
		}

		for(int j = 0 ; j < icmp.v[i].size() ; j++)
		{
			f2 = false;
			int temp = icmp.v[i][j].second;
			for(int k = 0 ; k < v[icmp.v[i][j].first].size() ; k++)
			{
				if(v[icmp.v[i][j].first][k].first == i)
				{
					temp += v[icmp.v[i][j].first][k].second;
					f2 = true;
					v[icmp.v[i][j].first][k].second = temp;
					break;
				}
			}
			if(!f2)
			{
				v[icmp.v[i][j].first].push_back(make_pair(i,temp));
			}
		}

		for(int j = 0 ; j < any.v[i].size() ; j++)
		{
			f4 = false;
			int temp = any.v[i][j].second;
			for(int k = 0 ; k < v[any.v[i][j].first].size() ; k++)
			{
				if(v[any.v[i][j].first][k].first == i)
				{
					temp += v[any.v[i][j].first][k].second;
					v[any.v[i][j].first][k].second = temp;
					f4 = true;
					break;
				}
			}
			if(!f1)
				v[any.v[i][j].first].push_back(make_pair(i,temp));
		}
	}
}



int main() {
	mp.clear();
	ifstream file;
	file.open("hosts.csv" , ios :: in);
	string line , addr , l;
	int label;
	getline(file , line);
	n = stoi(line);
	cout << "\nNumber of unique VM's in the subnet are : " << n << "\n";
	cout << endl;
	while(getline(file , line)) {
		stringstream s(line);
		getline(s , l , ',');
		getline(s , addr , '\n');
		label = stoi(l); 
		if(mp.find(addr) == mp.end()) {
			mp.insert({addr , label});
			rev_mp.insert({label , addr});
		}
	}
	/*for(auto it : mp) {
		cout << it.first <<  " " << it.second << "\n";
	}*/
	network tcp , udp , icmp,finalg,any;
	tcp.create("TCP",45);
	udp.create("UDP",50);
	icmp.create("ICMP",100);
	any.create("ANY",0);
	finalg.create_final(tcp , udp , icmp ,any);
	cout<<"Enter the source : ";
	string source;
	cin >> source;
	vector <int> dist = finalg.djikstras(mp[source]);
	vector <pair <int , string>> p;
	for(int i = 1 ; i <= n ; i++) {
		p.push_back(make_pair(dist[i],rev_mp[i]));
	}
	sort(p.begin() , p.end());
	cout << endl;
	cout<<setw(15)<<"Impact";
	cout<<setw(15)<<"IP address\n";
	for(auto it : p) {
		if(it.first==999999999)
		{
			cout <<setw(15)<<"Not Connected"<<setw(15)<< it.second << "\n";
		}
		else if(it.second==source)
		{
			cout <<setw(15)<<"Infected source"<<setw(15)<< it.second << "\n";
		}
		else
		{
			cout <<setw(15)<<it.first <<setw(15)<< it.second << "\n";
		}
		
	}
	return 0;
}
