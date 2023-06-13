#include <iostream>
#include <vector>
#include <random>

using namespace std;

struct Player {
    string name;
    int score;

    Player(const string& name, int score) : name(name), score(score) {}
};

struct SkipListNode {
    Player player;
    SkipListNode* next;
    SkipListNode* down;

    SkipListNode(const Player& player) : player(player), next(nullptr), down(nullptr) {}
};

class SkipList {
private:
    int max_level;
    SkipListNode* head;
    int levels;

public:
    SkipList() : max_level(16), head(new SkipListNode(Player("", INT_MIN))), levels(1) {}

    void insert(const Player& player) {
        SkipListNode* current = head;
        vector<SkipListNode*> update_nodes(max_level, nullptr);

        for (int level = levels - 1; level >= 0; --level) {
            while (current->next && current->next->player.score > player.score) {
                current = current->next;
            }
            update_nodes[level] = current;
            current = current->down;
        }

        SkipListNode* new_node = new SkipListNode(player);
        new_node->next = update_nodes[0]->next;
        update_nodes[0]->next = new_node;

        int level = 1;
        while (level < max_level && (random() % 2) == 0) {
            if (level < levels) {
                new_node->down = update_nodes[level]->next;
                update_nodes[level]->next = new_node;
            } else {
                SkipListNode* new_level_node = new SkipListNode(player);
                new_level_node->down = head;
                new_level_node->next = new_node;
                head = new_level_node;
                levels += 1;
            }

            new_node = new_level_node;
            level += 1;
        }
    }

    bool remove(const Player& player) {
        SkipListNode* current = head;
        bool found = false;

        for (int level = levels - 1; level >= 0; --level) {
            while (current->next && current->next->player.score > player.score) {
                current = current->next;
            }

            if (current->next && current->next->player.score == player.score) {
                SkipListNode* temp = current->next;
                current->next = current->next->next;
                delete temp;
                found = true;
            }

            current = current->down;
        }

        return found;
    }

    vector<Player> get_players() {
        vector<Player> players;
        SkipListNode* current = head->next;

        while (current) {
            players.push_back(current->player);
            current = current->next;
        }

        return players;
    }
};

int main() {
    SkipList skip_list;

    vector<Player> players = {
        Player("Said Abror", 100),
        Player("Abdullo", 75),
        Player("Rukhsora", 120),
        Player("Alibek", 90),
        Player("Asaloy", 80)
    };

    // Insert players into the skip list
    for (const auto& player : players) {
        skip_list.insert(player);
    }

    // Get and print players from the leaderboard
    vector<Player> leaderboard = skip_list.get_players();
    for (const auto& player : leaderboard) {
        cout << player.name << endl;
    }

    return 0;
}