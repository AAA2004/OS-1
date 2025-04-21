#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

// Data: object -> (domain -> permissions)
map<string, map<string, set<char>>> permissions;

// Domain and object registries
set<string> domainList;
set<string> objectList;

// Add domain
void createDomain(const string& name) {
    if (domainList.count(name)) {
        cout << "⚠️ Domain already exists.\n";
    } else {
        domainList.insert(name);
        cout << "✅ Domain '" << name << "' created.\n";
    }
}

// Add object
void createObject(const string& name) {
    if (objectList.count(name)) {
        cout << "⚠️ Object already exists.\n";
    } else {
        objectList.insert(name);
        cout << "✅ Object '" << name << "' created.\n";
    }
}

// Assign access rights
void allowAccess(const string& domain, const string& obj, const string& rights) {
    if (!domainList.count(domain) || !objectList.count(obj)) {
        cout << "❌ Domain or Object does not exist.\n";
        return;
    }
    for (char r : rights) {
        permissions[obj][domain].insert(r);
    }
    cout << "✅ Access rights granted.\n";
}

// Remove access rights
void denyAccess(const string& domain, const string& obj, const string& rights) {
    if (!domainList.count(domain) || !objectList.count(obj)) {
        cout << "❌ Domain or Object does not exist.\n";
        return;
    }
    for (char r : rights) {
        permissions[obj][domain].erase(r);
    }
    cout << "✅ Access rights revoked.\n";
}

// Delete domain
void deleteDomain(const string& domain) {
    if (!domainList.erase(domain)) {
        cout << "❌ Domain not found.\n";
        return;
    }

    for (auto& entry : permissions) {
        entry.second.erase(domain);
    }

    cout << "🗑️ Domain '" << domain << "' deleted.\n";
}

// Delete object
void deleteObject(const string& obj) {
    if (!objectList.erase(obj)) {
        cout << "❌ Object not found.\n";
        return;
    }

    permissions.erase(obj);
    cout << "🗑️ Object '" << obj << "' deleted.\n";
}

// Show current ACL table
void showPermissions() {
    cout << "\n📋 Current ACL Table:\n";
    for (const auto& [obj, domainRights] : permissions) {
        cout << "Object: " << obj << "\n";
        for (const auto& [domain, rights] : domainRights) {
            cout << "  Domain: " << domain << " → ";
            for (char r : rights) {
                cout << r;
            }
            cout << "\n";
        }
    }
    cout << endl;
}

int main() {
    string cmd;

    cout << "🔐 Access Control Manager — Fresh Version\n";
    cout << "Commands: new_domain, new_object, allow, deny, del_domain, del_object, show, quit\n";

    while (true) {
        cout << "\n> ";
        cin >> cmd;

        if (cmd == "new_domain") {
            string d;
            cin >> d;
            createDomain(d);
        } else if (cmd == "new_object") {
            string o;
            cin >> o;
            createObject(o);
        } else if (cmd == "allow") {
            string d, o, rights;
            cin >> d >> o >> rights;
            allowAccess(d, o, rights);
        } else if (cmd == "deny") {
            string d, o, rights;
            cin >> d >> o >> rights;
            denyAccess(d, o, rights);
        } else if (cmd == "del_domain") {
            string d;
            cin >> d;
            deleteDomain(d);
        } else if (cmd == "del_object") {
            string o;
            cin >> o;
            deleteObject(o);
        } else if (cmd == "show") {
            showPermissions();
        } else if (cmd == "quit") {
            break;
        } else {
            cout << "❌ Unknown command.\n";
        }
    }

    return 0;
}
