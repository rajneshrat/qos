#define GET_TOTAL_USERS 0

struct qos_application
{
   unsigned long MBytesDownloaded;
   unsigned int MaxKBpsAllowed;
   int port;
   struct qos_application *next;
};

struct qos_user
{
   int UserId;
   char Mac[6];
   char UserName[100];
   struct qos_appliation *Applications;
   unsigned int TotalBytesUploaded;
};

struct global_qos
{
   struct qos_user *UserList;
   unsigned int TotalMBytesUploaded;
   int MaxKBpsAllowed; //this is the overall maximum limit allowed through engine.
};
