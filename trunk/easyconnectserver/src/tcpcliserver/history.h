

struct commandentry
{
  char* Command;
  CommandEntry* Next;
  CommandEntry* Previous;
};
typedef struct commandentry CommandEntry;


struct commandhistory
{
  CommandEntry* Begin;
  CommandEntry* End;
  CommandEntry* Current;
  
  int Socket;
  
  CommandHistory* Next
  CommandHistory* Previous;  
};
typedef struct commandhistory CommandHistory;

struct histories
{
  CommandHistory* Begin;
  CommandHistory* End;
};
typedef struct histories Histories;



