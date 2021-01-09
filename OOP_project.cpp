#include<iostream>
#include<cstring>
#include<fstream>
#include<iomanip>


using namespace std;

unsigned int user_no;                                       // stores ("the number of user"-1) as UIDs start from zero
// files for saving the graph to disk
fstream fgraph;
fstream cgraph;
fstream infor;

// classes and member functions start
class fgraph_node {

public:
	int uid;
	fgraph_node *friend_link = nullptr;
};
class cgraph_node {

public:
	int uid;
	cgraph_node *disc_link = nullptr;
};

// enum skill{production, research, marketing, human_resource, accounting_finance};

class user {
	char name[52];
	// skill is abilty to work in other group
	int skill1;
	int skill2;
	int skill3;
	char password[51];

public:
	int uid;
	fgraph_node *friend_link = nullptr;
	cgraph_node *disc_link = nullptr;
	void disp_info() {
	cout << "name = " << name << "\tskill-1 = " << skill1 << "\tskill-2 = " << skill2 << "\tskill-3 = " << skill3 << "\tuser id = " << uid;
	}
	void input(int userid)
	{
		int temp;
		cout << "enter name => ";
		cin>>name;
		cout << "enter a PASSWORD(max 50 words) => "; cin>>password;
		cout << "enter your skill set :\n";
		cout << "0->production\t1->research\t2->marketing\t3->human_resource\t4->accounting & finance\nskill-1 => "; cin>>skill1;
		cout << "skill-2 => "; cin>>skill2;
		cout << "skill-3 => "; cin>>skill3;
		uid = userid;
		cout << "your UID is = " << uid;
	}
	void deleteo()
	{
		int temp;
		strcpy(name, "DELETED");
		skill1 = -1;
		skill2 = -1;
		skill3 = -1;
	}
	bool check_pass(char *r)
	{
		if (strcmp(r, password)) {return 0;}
		else {return 1;}
	}
	void operator =(user u)                                    // operator function as otherwise "=" will copy all the data including the pointers variable
	{                                                          // without any memory leading to segmentation fault
		uid = u.uid;
		strcpy(name, u.name);
		skill1 = u.skill1;
		skill2 = u.skill2;
		skill3 = u.skill3;
		strcpy(password, u.password);
	}
	bool check_skill(int skillno)
	{
		if (skill1 == skillno || skill2 == skillno || skill3 == skillno) {return 1;}
		return 0;
	}
	int return_userid()
	{
	    return (uid);
	}
};
user usr[200];                                               // global array for storing the user objects

class gmem {

public:
	int uid;
	int compatiblity = 0;                                      // if this value is high then the user is more likely to be in any group
	void operator =(gmem m)                                    // not really neccessary but helps in keeping track
	{
		uid = m.uid;
		compatiblity = m.compatiblity;
	}
};

// classes and member function declaration end

void display()
{
// 	user usr;
// 	infor.seekg(0, ios::beg);
// 	while (infor.good())
// 	{
// 		infor.read((char *) &usr, sizeof(usr));
// 		usr.disp_info();
// 	}
	for (int i = 0; i <= user_no; i++) {
		usr[i].disp_info();
		cout << endl;
	}
}

bool save_link(int to_uid, int input_uid, fstream &file)
{
	file.seekp(0, ios::end);
	if (file.good()) {file << setw(3) << to_uid << " " << setw(3) << input_uid << endl;}
	else {return 0;}
	return 1;
}

bool delete_link(int to_uid, int input_uid, fstream &file)
{
    int u_id, fr_id;
    char d;
    file.seekp(0, ios::beg);
    file.seekg(0, ios::beg);
//     cout<<"enter user id of user who want to remove"<<endl;
//     cin>>u_id;
    while(!file.eof()) {
		file >> u_id >>fr_id;
		if (u_id == to_uid && fr_id == input_uid) {
			file.seekp(file.tellg(), ios::beg); file.seekp(-7, ios::cur);
			file << "       ";
			return 1;
		}
	}
	return 0;
//     while(!file.eof())
//     {
//         if(u_id == to_uid)
//         {
//              file.seekp(-6, ios::cur);
//              file<<"   "<< "\t" <<"   "<<endl;
//              return 1;
//         }
// 
//     }

}

bool f_create_link(int, int);                               // (prototype)to create link in memory for fast operations
bool c_create_link(int, int);                               // (prototype)to create link in memory for fast operations

bool add_user()
{
	fgraph.open("friend_graph.txt", ios::in | ios::ate | ios::out);
	cgraph.open("discordant_graph.txt", ios::in | ios::ate | ios::out);
	infor.open("usrs.dat", ios::binary | ios::in | ios::ate | ios::out);
	user u;
	user_no++;                                                 // incrementing as a new user is added
	int input_uid;
	bool temp;
	usr[user_no].input(user_no);
	cout << "\nenter the uids of users the user is friends with(type -1 to exit) : \n" << "friend's uid => ";
	cin>>input_uid;
	while (input_uid != -1) {
	// here fgraph is the friend graph's fstream object
		if (!save_link(usr[user_no].uid, input_uid, fgraph)) {cout << "Link saving failed"; break; return 0;}
		if (!f_create_link(usr[user_no].uid, input_uid)) {cout << "link creation in memory failed"; break; return 0;}
		cout << "friend's uid => ";
		cin>>input_uid;
	}
	cout << "\nenter the uids of users the user is discordant with(type -1 to exit) : \n" << "discordant's uid => ";
	cin>>input_uid;
	while (input_uid != -1) {
	// here cgraph is the discordant graph's fstream object
		if (!save_link(usr[user_no].uid, input_uid, cgraph)) {cout << "Link saving failed"; break; return 0;}
		if (!c_create_link(usr[user_no].uid, input_uid)) {cout << "link creation in memory failed"; break; return 0;}
		cout << "discordant's uid => ";
		cin>>input_uid;
	}
	fgraph.close(); cgraph.close();
	infor.seekp(0, ios::beg);
	if (infor.good()) {
		infor.seekp(0, ios::end);
		u = usr[user_no];
		infor.write((char *) &u, sizeof(u));
		infor.close();
	}
	else {cout << "File access failed\n"; infor.close(); return 0;}
	return 1;
}

bool f_create_link(int userid, int friendid)                // to create link in memory for fast operations
{
	fgraph_node *ftemp;
	ftemp = usr[userid].friend_link;
	if (usr[userid].friend_link == nullptr) {
		usr[userid].friend_link = new fgraph_node;
		usr[userid].friend_link->uid = friendid;
	}
	else {
		while (ftemp->friend_link != nullptr) {ftemp = ftemp->friend_link;}
		ftemp->friend_link = new fgraph_node; ftemp->friend_link->uid = friendid;
	}
	return 1;
}

bool f_delete_link(int userid, int friendid)
{
	int place = 0;
	fgraph_node *ftemp, *swap;
	ftemp = usr[userid].friend_link;
	while (ftemp != nullptr) {
		if (ftemp->uid == friendid) {
			break;
		}
		place++;
	}
	if (place == 0) {return 0;}
	ftemp = usr[userid].friend_link;
	while (place > 0) {
		ftemp = ftemp->friend_link;
		place--;
	}
	swap = ftemp->friend_link->friend_link;
	ftemp->friend_link = swap;
	return 1;
}

bool c_delete_link(int userid, int discid)
{
	int place = 0;
	cgraph_node *ctemp, *swap;
	ctemp = usr[userid].disc_link;
	while (ctemp != nullptr) {
		if (ctemp->uid == discid) {
			break;
		}
		place++;
	}
	if (place == 0) {return 0;}
	ctemp = usr[userid].disc_link;
	while (place > 0) {
		ctemp = ctemp->disc_link;
		place--;
	}
	swap = ctemp->disc_link->disc_link;
	ctemp->disc_link = swap;
	return 1;
}

bool c_create_link(int userid, int discid)                  // to create link in memory for fast operations
{
	cgraph_node *ctemp;
	ctemp = usr[userid].disc_link;
	if (usr[userid].disc_link == nullptr) {
		usr[userid].disc_link = new cgraph_node;
		usr[userid].disc_link->uid = discid;
	}
	else {
		while (ctemp->disc_link != nullptr) {ctemp = ctemp->disc_link;}
		ctemp->disc_link = new cgraph_node; ctemp->disc_link->uid = discid;
	}
	return 1;
}


bool initialize_database()
{
	fgraph.open("friend_graph.txt", ios::in | ios::ate | ios::out);
	cgraph.open("discordant_graph.txt", ios::in | ios::ate | ios::out);
	infor.open("usrs.dat", ios::binary | ios::in | ios::ate | ios::out);
	user u;
	int i = -1, userid, friendid, discid;                     // friendid - friend's id, discid - discordant's id
	fgraph_node *ftemp;
	cgraph_node *ctemp;
	infor.seekg(0, ios::beg);
	while (!infor.eof()) {
		i++;
		infor.read((char *) &u, sizeof(u));
		usr[i] = u;
	}
	i--;
	cout << " " << i;
	user_no = i;
	if (i == -1) {cout << "file access failed"; return 0;}
	fgraph.seekg(0, ios::beg);
	while (!fgraph.eof()) {
		fgraph>>userid>>friendid;
		ftemp = usr[userid].friend_link;
		if (usr[userid].friend_link == nullptr) {
			usr[userid].friend_link = new fgraph_node;
			usr[userid].friend_link->uid = friendid;
		}
		else {
			while (ftemp->friend_link != nullptr) {ftemp = ftemp->friend_link;}
			ftemp->friend_link = new fgraph_node; ftemp->friend_link->uid = friendid;
		}
	}
	ftemp->friend_link = nullptr;                              // to erase an extra record at the end
	cgraph.seekg(0, ios::beg);
	while (!cgraph.eof()) {
		cgraph>>userid>>discid;
		ctemp = usr[userid].disc_link;
		if (usr[userid].disc_link == nullptr) {
			usr[userid].disc_link = new cgraph_node;
			usr[userid].disc_link->uid = discid;
		}
		else {
			while (ctemp->disc_link != nullptr) {ctemp = ctemp->disc_link;}
			ctemp->disc_link = new cgraph_node; ctemp->disc_link->uid = discid;
		}
	}
	ctemp->disc_link = nullptr;                                // to erase an extra record at the end
	infor.close(); fgraph.close(); cgraph.close();
	if (ftemp != nullptr) {return 1;}                          // statement to check if the Initialization was successful
	else {return 0;}
}

void display_links()                                        // to display all the links when in admin mode
{
	fgraph_node *ftemp;
	cgraph_node *ctemp;
	for (int i = 0; i <= user_no; i++) {
		usr[i].disp_info();
		ftemp = usr[i].friend_link;
		cout << "\n\n\tthe user has the following friends :";
		while (ftemp != nullptr) {
			cout<< "\n\t"; usr[ftemp->uid].disp_info();
			ftemp = ftemp->friend_link;
		}
		ctemp = usr[i].disc_link;
		cout << "\n\tthe user has the following discordinators :";
		while (ctemp != nullptr) {
			cout<< "\n\t"; usr[ctemp->uid].disp_info();
			ctemp = ctemp->disc_link;
		}
		cout << "\n\n";
	}
}

void most_links()
{
	fgraph_node *ftemp;
	cgraph_node *ctemp;
	int fmax = 0, cmax = 0, temp1, temp2;
	for (int i = 0; i <= user_no; i++) {
		int fcount = 0, ccount = 0;
		ftemp = usr[i].friend_link;
		while (ftemp != nullptr) {
			fcount++;
			ftemp = ftemp->friend_link;
		}
		ctemp = usr[i].disc_link;
		while (ctemp != nullptr) {
			ccount++;
			ctemp = ctemp->disc_link;
		}
		if (fcount > fmax) {fmax = fcount; temp1 = i;}
		if (ccount > cmax) {cmax = ccount; temp2 = i;}
	}
	cout <<"the following user has the most friends :\n";
	usr[temp1].disp_info();
	cout << "       with " << fmax << " friends\n";
	cout <<"the following user has the most discordants :\n";
	usr[temp2].disp_info();
	cout << "       with " << cmax << " discordants\n";
}

bool is_friend(int uid, int other_uid)
{
	fgraph_node *ftemp;
	ftemp = usr[uid].friend_link;
	while (ftemp != nullptr) {
		if (ftemp->uid == other_uid) {return 1;}
		ftemp = ftemp->friend_link;
	}
	return 0;
}

bool is_discordant(int uid, int other_uid)
{
	cgraph_node *ctemp;
	ctemp = usr[uid].disc_link;
	while (ctemp != nullptr) {
		if (ctemp->uid == other_uid) {return 1;}
		ctemp = ctemp->disc_link;
	}
	return 0;
}

void mkgroup(int req1, int req2 = -1, int req3 = -1)        // req is for requirement
{
	int i = 0, strength, j = -1, k;
	gmem gruid[30], temp;
	cout << "\nhow many people do you want in the group => ";
	cin>>strength;
	if (req2 == -1 && req3 == -1) {
		for (i = 0; i <= user_no; i++) {
			if (usr[i].check_skill(req1)) {j++; gruid[j].uid = i;}
		}
	}
	else if (req3 == -1) {
		for (i = 0; i <= user_no; i++) {
			if (usr[i].check_skill(req1) && usr[i].check_skill(req2)) {j++; gruid[j].uid = i;}
		}
	}
	else {
		for (i = 0; i <= user_no; i++) {
			if (usr[i].check_skill(req1) && usr[i].check_skill(req2) && usr[i].check_skill(req3)) {j++; gruid[j].uid = i;}
		}
	}
	if (j == -1) {cout << "no user has the required skill-set\n"; return;}
	for (k = 0; k <= j; k++) {
		for (i = 0; i <= j; i++) {
			if (is_friend(gruid[k].uid, gruid[i].uid)) {gruid[i].compatiblity++;}
			if (is_discordant(gruid[k].uid, gruid[i].uid)) {gruid[i].compatiblity--;}
		}
	}
	while (j > strength-1) {
		gmem min;
		int index_of_min = 0;
		min = gruid[0];
		for (int i = 0; i <= j; i++) {
			if (gruid[i].compatiblity < min.compatiblity) {index_of_min = i; min = gruid[i];}
		}
		gruid[index_of_min] = gruid[j];
		gruid[j] = min;
		j--;
	}
	cout << "\nRecommended group members are as follows :\n";
	for (int i = 0; i <= j; i++) {
// 		cout << gruid[i].uid <<"\t" << i << "\n";                 // just for debugging
		cout << "\t";
		usr[gruid[i].uid].disp_info(); cout << endl;
	}
}

bool admin_algos()
{
	int ans, y[3], t, i;                                           // for user input to menu
// 	infor.seekg(-sizeof(temp1), ios::end);
// 	if (!infor.good()) {cout << "not good";}
// 	infor.read((char *) & temp1, sizeof(temp1));
// 	user_no = temp1.uid;
// 	cout << usr[0].uid;
    do
    {
//      system("clear");
		cout << "\n\t\tSOCIAL NETWORK DATABASE\n\n";
		cout << "1.display all links\n2.most liked and disliked user\n3.suggest groups\n4.Back to main menu\n\n==>"; cin >>ans;
		switch(ans)
		{
			case 1:
				display_links();
				break;
			case 2:
				most_links();
				break;
			case 3:
				cout << "\nenter the required skill-set below:\n";
				cout << "0->production\t1->research\t2->marketing\t3->human_resource\t4->accounting & finance\nenter the following in accordance :";
				for (i = 0; i<3 && y[i-1] != -1; i++) {
					cout << "\nenter 0 or 1 or 2 or 3 or 4 or -1(to stop)........";
					cin>>y[i];
				}
				if(i == 1)
					mkgroup(y[0]);
				else if (i == 2)
					mkgroup(y[0], y[1]);
				else if (i == 3)
					mkgroup(y[0], y[1], y[2]);
				break;
		}
	}while(ans == 1 || ans == 2 || ans == 3);
	return 1;
}

void modify_user(int userid)
{
    int choice, choice_f, choice_d,friend_id,discordant_id;
    cout<<"\n1. Add or Remove a friend "<<endl;
    cout<<"2. Add or Remove a discordant "<<endl;
    cout<<"3. edit my skills and password "<<endl;
    cout<<"enter your choice -> ";
    cin>>choice;
    if (choice==1)
    {
		fgraph.open("friend_graph.txt", ios::in | ios::ate | ios::out);
        cout<<"enter user id of friend: ";
        cin>>friend_id;
        cout<<"1. add friend"<<endl;
        cout<<"2. remove friend"<<endl;
        cout<<"enter your choice"<<endl;
        cin>>choice_f;
        if(choice_f==1)
        {
            if(save_link(userid, friend_id, fgraph)) {
                cout<<"friend added successfully"<<endl;
                f_create_link(userid, friend_id);
			}

        }
        else if (choice_f == 2)
        {
            if(delete_link(userid,friend_id, fgraph)) {
                cout<<"friend removed successfully"<<endl;
                f_delete_link(userid, friend_id);
			}    
        }
    }
    else if(choice == 2)
    {
		cgraph.open("discordant_graph.txt", ios::in | ios::ate | ios::out);
        cout<<"enter user id of discordant: "<<endl;
        cin>>discordant_id;
        cout<<"1. add discordant"<<endl;
        cout<<"2. remove discordant:"<<endl;
        cin>>choice_d;
        if(choice_d==1)
        {
            if(save_link(userid, discordant_id, cgraph)) {
                cout<<"discordant added successfully"<<endl;
                c_create_link(userid, discordant_id);
			}
        }
        else
        {
            if(delete_link(userid, discordant_id, cgraph)) {
                cout<<"discordant removed successfully"<<endl;
                c_delete_link(userid, discordant_id);
			}
        }
    }
    else if (choice == 3)
    {
        int user_id;
        user usr;
        fstream file3;
        file3.open("usrs.dat", ios::in | ios::out | ios::ate | ios::binary);
        cout<<"enter your user id"<<endl;
        cin>>user_id;
        while( file3.read((char *)&usr, sizeof(usr)))
        {
            if( usr.return_userid()== user_id)
           {
               usr.input( user_id);
               file3.seekg( -sizeof(usr), ios::cur);
               file3.write( (char *)&usr, sizeof(usr));
           }

       }
    }
    infor.close(); fgraph.close(); cgraph.close();
}

bool delete_user(int user_id)
 {
	 fgraph.open("friend_graph.txt", ios::in | ios::ate | ios::out);
	 cgraph.open("discordant_graph.txt", ios::in | ios::ate | ios::out);
	 char pass[21];
     user usr;
     int input_uid;
     bool temp1;

      //int user_id;
     cout<<"Enter your password => ";
     cin>>pass;
	if (!(::usr[user_id].check_pass(pass))) {cout << "password incorrect\n"; return 0;}
   //  cout<<" Enter the user id the user is friend with(type -1 to exit)"<<endl;
    // cin>>input_uid;

     /*    temp1= delete_link(usr.uid, user_id,fgraph);
         if(temp1 ==1)
         {
             cout<<"link is deleted"<<endl;
             return 0;
         }
         else
            cout<<"user id is not matched, hence link is not deleted"<<endl;

    // cout<<" Enter the user id the user is concordant with(type -1 to exit)"<<endl;
    // cin>>input_uid;

         temp1= delete_link(usr.uid, user_id,cgraph);
         if(temp1 ==1)
         {
             cout<<"link is deleted"<<endl;
             return 0;
         }
         else
            cout<<"user id is not matched, hence link is not deleted"<<endl;*/


     fstream file1;
     fstream file2;
     file1.open("usrs.dat", ios::in |ios::binary);
     file2.open("usrs_temp.dat", ios::out | ios::binary);



     while ( file1.read((char *)&usr, sizeof(usr)))
      {
          if(usr.return_userid()!= user_id)
            file2.write((char *)&usr, sizeof(usr));
      }
      file1.close();
      file2.close();
      remove("usrs.dat");
      rename("usrs_temp.dat","usrs.dat");
		::usr[user_id].deleteo();
		for (int i = 0; i<user_no; i++) {
			if (is_discordant(user_id, i)) {delete_link(user_id, i, cgraph); c_delete_link(user_id, i);}
			if (is_friend(user_id, i)) {delete_link(user_id, i, fgraph); f_delete_link(user_id, i);}
		}
 }

// main function

int main()
{
    cout << "PASSWORD of all the accounts in the sample database is same as their NAME and adminstrative PASSWORD is admin\n";
	bool tar = 0;
	infor.open("usrs.dat", ios::binary | ios::in | ios::ate | ios::out);
	infor.close();
	if (!initialize_database()) {cout << "\nInitialization failed"; exit(0);}
	else {cout << "Database Initialized successfully\n";}
	user temp1;
	int ans, y, t;                                                   // for user input to menu
	char passwd[20];

    do
    {
//      system("clear");
		cout << "\n\t\tSOCIAL NETWORK DATABASE\n\n";
		cout << "1.Administrator\n2.Add new user\n3.Login\n4.display all users\n5.exit\n\n==>"; cin >>ans;
		switch(ans)
		{
			case 1:
				cout << "enter the administrative password: ";
				cin>>passwd;
				if (strcmp(passwd, "admin")) {cout << "password incorrect\n"; break;}
				admin_algos();
				break;

			case 2:
				if (add_user()) {
					cout << "new user added successfully\n";
				}
				else {cout << "user could not be added\n";}
				cin.get();
				break;

			case 3:
				cout << "enter your user id(UID) => "; cin>>t;
				usr[t].disp_info();
				for (int i = 0; i<3 && tar == 0; i++) {
					cout << "\nenter your user password: ";
					cin>>passwd;
					if (usr[t].check_pass(passwd)) {tar = 1;}
					else {cout << "password incorrect\n";}
				}
				if (tar == 0) {break;}
				int ch;
				cout<<"\n1. edit my user profile "<<endl;
				cout<<"2. delete my id "<<endl;
				cout<<"3. display my friends and discordants "<<endl;
				cout<<"Enter your choice(1 or 2 or 3)"<<endl;
				cin>>ch;
				switch(ch)
				{
				    case 1: modify_user(t);
				            break;
                    case 2: delete_user(t);
                            break;
					case 3: 
						cout << "\n your friends are :\n";
						for(int i = 0; i <= user_no; i++) {
							if (is_friend(t, i)) {usr[i].disp_info(); cout << endl;}
						}
						cout << "\n your discordants are :\n";
						for(int i = 0; i <= user_no; i++) {
							if (is_discordant(t, i)) {usr[i].disp_info(); cout << endl;}
						}
				}
				break;

			case 4:
				display();
				cin.get();
				break;

		}
	} while(ans == 1 || ans == 2 || ans == 3 || ans == 4);
	infor.close();
}

