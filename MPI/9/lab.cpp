#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>

enum money_type {
    DOLLAR,
    YUAN,
    FUNT,
    NUM_OF_MONEY_TYPE,
};

struct debt_t {
    size_t from;
    size_t to;
    int value[NUM_OF_MONEY_TYPE];
};

struct money_t {
    size_t from;
    size_t to;
    money_type type;
    int value;
};

const char* type_to_str(money_type money_type) 
{
    switch (money_type)
    {
    case DOLLAR:
        return "dollar";
    case YUAN:
        return "yuan";
    case FUNT:
        return "funt";
    default:
        return "<INVALID MONEY TYPE>";
    }
}

void print_debt(const debt_t& debt)
{
    printf("Bank %u has debt to bank %u: %d dollars, %d yuan, %d funt\n", debt.from, debt.to, 
        debt.value[DOLLAR], debt.value[YUAN], debt.value[FUNT]);
}

std::vector<debt_t> random_debts(size_t from, size_t num_of_banks) 
{
    std::vector<debt_t> res;

    for (int i = 1; i <= num_of_banks; ++i) {
        if (i == from) {
            continue;
        }

        debt_t debt = {from, i};

        for (int type = 0; type < NUM_OF_MONEY_TYPE; ++type) {
            debt.value[type] = std::abs(rand() % 10000);
        }

        res.push_back(debt);    
    }
    return res;
}

int main( int argc, char **argv )
{
    double t, t2;
    int i, bank, size;
    int num_of_banks;



    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &bank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_of_banks);

    srand(time(0) + bank);

    if (num_of_banks < 4) {
        std::cerr << "invalid num of banks, must be more than 3";
        return 1;
    }

    t=MPI_Wtime(); // фиксация времени «начала посылки»,

    // 0 bank is Big Bank
    if (bank != 0) {
        std::vector<debt_t> debts = random_debts(bank, num_of_banks - 1);
        money_t money;
        std::map<size_t, size_t> money_need_to_rcv;

        std::for_each(debts.begin(), debts.end(), [](const debt_t& debt) 
                                                    {
                                                        print_debt(debt);
                                                    });
        
        printf("Bank %u sending info about debts to Big Bank\n", bank);
        MPI_Send(&debts[0], sizeof(debt_t) * debts.size(), MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&debts[0], sizeof(debt_t) * debts.size(), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);
        printf("Bank %u received info about debts from Big Bank\n", bank);


        for (auto it = debts.begin(); it != debts.end(); ++it) {
            for (int type = 0; type < NUM_OF_MONEY_TYPE; ++type) {
                if (it->value[type] > 0) {
                    printf("Bank %u send to bank %u %d %s\n", bank, it->to, it->value[type], type_to_str((money_type) type));
                    money.from = it->from;
                    money.to = it->to;
                    money.type = (money_type) type;
                    money.value = it->value[type];
                    MPI_Send(&money, sizeof(money_t), MPI_BYTE, money.to, 0, MPI_COMM_WORLD);
                } else if (it->value[type] < 0) {
                    ++money_need_to_rcv[it->to];
                }
            }
        }

        for (auto it = money_need_to_rcv.begin(); it != money_need_to_rcv.end(); ++it) {
            while (it->second-- > 0) {
                MPI_Recv(&money, sizeof(money_t), MPI_BYTE, it->first, 0, MPI_COMM_WORLD, &status);
                printf("Bank %u received from bank %u %d %s\n", bank, money.from, money.value, type_to_str(money.type));
            }
        }
    } else {
        std::vector<std::vector<debt_t>> banks_debts;
        std::vector<debt_t> debts(num_of_banks - 2);

        printf("Big Bank waiting for info from %u banks\n", num_of_banks - 1);

        for (int i = 1; i < num_of_banks; ++i) {
            MPI_Recv(&debts[0], sizeof(debt_t) * debts.size(), MPI_BYTE, i, 0, MPI_COMM_WORLD, &status);
            printf("Big Bank receive info about debts from bank %d\n", i);
            banks_debts.push_back(debts);
        }

        for (auto bank = banks_debts.begin(); bank != banks_debts.end(); ++bank) {
            for (auto debt = bank->begin(); debt != bank->end(); ++debt) {
                auto peer_bank = std::find_if(banks_debts.begin(), banks_debts.end(), 
                                            [debt](const std::vector<debt_t>& debts) {
                                                return debts[0].from == debt->to;
                                            });
                auto peer_debt = std::find_if(peer_bank->begin(), peer_bank->end(),
                                            [debt](const debt_t& debt_) {
                                                return debt_.to == debt->from;
                                            });
                for (int type = 0; type < NUM_OF_MONEY_TYPE; ++type) {
                    int tmp = debt->value[type] - peer_debt->value[type];
                    peer_debt->value[type] = peer_debt->value[type] - debt->value[type];
                    debt->value[type] = tmp;
                }
            }
            printf("Big Bank send updated info about debts to bank %lu\n", (*bank)[0].from);
            MPI_Send(&(*bank)[0], sizeof(debt_t) * bank->size(), MPI_BYTE, (*bank)[0].from, 0, MPI_COMM_WORLD);
        }
    }


    t2=MPI_Wtime(); // фиксация времени «окончания приема»,

    printf("\n From processor %d\n Time=%le\n", bank,(t2-t)/100); // вывод времени,

    MPI_Finalize();
    return 0;
}