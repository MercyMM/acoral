#ifndef FS_FAT_H
#define FS_FAT_H
#ifndef MAX_OPEN_FILES
#define MAX_OPEN_FILES              10          //����ͬʱ�򿪵��ļ���Ŀ
#endif

#ifndef DISK_CACHE_SIZE
#define DISK_CACHE_SIZE             512         //�ļ�ϵͳCache��С(�ֽ�),Ӧ��������������ռ�ֽ���
#endif

#ifndef MAX_DISK_CACHES
#define MAX_DISK_CACHES             100          //�ļ�ϵͳCache��Ŀ
#endif

#ifndef MAX_DRIVES
#define MAX_DRIVES                  1           //����ͬʱ���ص��߼���������Ŀ
#endif

typedef acoral_32 HANDLE;

#define Not_Open_FILE               -1


/* �ײ��������� */
#define DISK_INIT                   0
#define DISK_CLOSE                  1
#define DISK_READ_SECTOR            2
#define DISK_WRITE_SECTOR           3
#define DISK_READ_SUPER_SECTOR           4
/* �ײ���������ֵ */
#define DISK_READ_OK                0x01
#define DISK_READ_NOT_OK            0x80
#define DISK_WRITE_OK               0x02
#define DISK_WRITE_NOT_OK           0x82
#define DISK_INIT_OK                0x03
#define DISK_INIT_NOT_OK            0x83
#define BAD_DISK_COMMAND            0xffff

/* ��������ֵ */
#define RETURN_OK                   0x00    /* �����ɹ�         */
#define NOT_FIND_DISK               0x01    /* �߼��̲�����     */
#define DISK_FULL                   0x02    /* �߼�����         */
#define SECTOR_NOT_IN_CACHE         0x03    /* ����û�б�cache  */
#define NOT_EMPTY_CACHE             0x04    /* û�п���cache    */
#define SECTOR_READ_ERR             0x05    /* ����������       */
#define CLUSTER_NOT_IN_DISK         0x06    /* �߼�����û�д˴�  */
#define NOT_FIND_FDT                0x07    /* û�з����ļ�(Ŀ¼)*/
#define NOT_FAT_DISK                0x08    /* ��FAT�ļ�ϵͳ    */
#define FDT_OVER                    0x09    /* FDT����������Χ  */
#define FDT_EXISTS                  0x0a    /* �ļ�(Ŀ¼)�Ѿ�����*/
#define ROOT_FDT_FULL               0x0b    /* ��Ŀ¼��         */
#define DIR_EMPTY                   0x0C    /* Ŀ¼��           */
#define DIR_NOT_EMPTY               0x0d    /* Ŀ¼����         */
#define PATH_NOT_FIND               0x0e    /* ·��δ�ҵ�       */
#define FAT_ERR                     0x0f    /* FAT������       */
#define FILE_NAME_ERR               0x10    /* �ļ�(Ŀ¼)������ */
#define FILE_EOF                    0x11    /* �ļ�����        */
#define FILE_LOCK                   0x12    /* �ļ�������       */
#define NOT_FIND_FILE               0x13    /* û�з���ָ���ļ� */
#define NOT_FIND_DIR                0x14    /* û�з���ָ��Ŀ¼ */
#define NOT_RUN                     0xfd    /* ����δִ��       */
#define BAD_COMMAND                 0xfe    /* ��������       */
#define PARAMETER_ERR               0xff    /* �Ƿ�����        */

/* δʹ�õ��߼���  */
#define EMPTY_DRIVE                 0xff

/* FAT���� */
#define FAT12                       0
#define FAT16                       1
#define FAT32                       2

/* FDT�ļ����� */
#define ATTR_READ_ONLY              0x01
#define ATTR_HIDDEN                 0x02
#define ATTR_SYSTEM                 0x04
#define ATTR_VOLUME_ID              0x08
#define ATTR_DIRECTORY              0x10
#define ATTR_ARCHIVE                0x20

/* ������ */
#define EMPTY_CLUS                  0
#define EMPTY_CLUS_1                1
#define BAD_CLUS                    0x0ffffff7L
#define EOF_CLUS_1                  0x0ffffff8L
#define EOF_CLUS_END                0x0fffffffL

/* FDT���� */
#define EMPTY_FDT                   0
#define DEL_FDT                     ((char)(0xe5))
#define ESC_FDT                     0x05

/* Cache״̬ */
#define CACHE_READED                0x02
#define CACHE_WRITED                0x01

/* �ļ�ָ��״̬ */
#define FILE_FLAGS_READ             1 << 0          // �ɶ�
#define FILE_FLAGS_WRITE            1 << 1          // ��д

/* �ļ�ָ�������ʽ */
#define SEEK_SET    0               // ���ļ���ʼ���ƶ��ļ�ָ��
#define SEEK_CUR    1               // ���ļ���ǰλ���ƶ��ļ�ָ��
#define SEEK_END    2               // ���ļ�β�ƶ��ļ�ָ��


/* ���ײ�������д����Ĳ����ṹ�� */
typedef struct _Disk_RW_Parameter
{
	acoral_u8 		Drive;			//������������
    acoral_u32      SectorIndex;    // ����������
    acoral_u32      RsvdForLow;     // �������ײ�����������_Disk_Info�п������� 
    acoral_u8       *Buf;           // ���ݴ洢λ��
}Disk_RW_Parameter;

/* �ļ�ϵͳcache���ݽṹ */
typedef struct _Disk_Cache
{
    acoral_u8       Drive;          // ��������,EMPTY_DRIVEΪ����
    acoral_u8       Flag;           // ״̬
//    acoral_u16      RW_ID;          // ��дID,�����ͷ�Cache
//    acoral_u8       Lock;           // �Ƿ���������
    acoral_u32      SecIndex;       // �������������
    struct _Disk_Cache *NextHash;
    struct _Disk_Cache *FrontHash;
    struct _Disk_Cache *NextFreeList;
//    struct _Disk_Cache *FrontFreeList;
    acoral_u8       buf[DISK_CACHE_SIZE];
                                // ������
}Disk_cache;

/* �߼�����Ϣ�����ݽṹ */
typedef struct _Disk_Info
{
    acoral_u8       Drive;          // �߼��������ţ�EMPTY_DRIVEΪ��δ����
    acoral_u8       FATType;        // ���ͣ�FAT12��FAT16��FAT32
    acoral_u8       SecPerClus;     // ÿ��������
    acoral_u8       NumFATs;        // FAT����Ŀ

	acoral_id		DevId;

    acoral_u32      SecPerDisk;     // �߼�����������������
    acoral_u32      BytsPerSec;     // ÿ�����ֽ���
    acoral_u32      RootDirTable;   // ��Ŀ¼��ʼ�����ţ�FAT32Ϊ��ʼ�غţ�
    acoral_u32      RootSecCnt;     // ��Ŀ¼ռ��������
    acoral_u32      FATStartSec;    // FAT����ʼ������
    acoral_u32      FATSecCnt;      // ÿ��FATռ��������
    acoral_u32      DataStartSec;   // ��������ʼ������
    acoral_u32      ClusPerData;    // ��������������
    acoral_u32      PathClusIndex;  // ��ǰ·����FDT����ʼ�غ�, 0Ϊ��Ŀ¼ 
    acoral_u32      RsvdForLow;     // �������ײ��������� 
    acoral_u16  (* DiakCommand)(acoral_u8 Cammand, void *Parameter);
                                // ��Ӧ����������
}Disk_Info;

/* Ŀ¼�����ݽṹ */
typedef struct _FDT
{
    acoral_char    Name[11];           //���ļ������ļ���
    acoral_u8   Attr;               //�ļ�����
    acoral_u8   NTRes;              //������NT
    acoral_u8   CrtTimeTenth;       //����ʱ�䣨fat16������
    acoral_u16  CrtTime;            //����ʱ�䣨fat16������
    acoral_u16  CrtDate;            //�������ڣ�fat16������
    acoral_u16  LstAccDate;         //���������ڣ�fat16������
    acoral_u16  FstClusHI;          //��ʼ�غŸ������ֽڣ�fat16������
    acoral_u16  WrtTime;            //���дʱ��
    acoral_u16  WrtDate;            //���д����
    acoral_u16  FstClusLO;          //��ʼ��(cluster)�ŵ������ֽ�
    acoral_u32  FileSize;           //�ļ���С
} FDT;

/* �ļ���Ϣ�ṹ�� */
typedef struct _FILE
{
    acoral_u8       Flags;          // һЩ��־
    acoral_char        Name[11];       // �ļ���
    acoral_u8       Drive;          // �ļ����ڴ���
    acoral_u32      DirClus;        // ����Ŀ¼��ʼ�غ�
    acoral_u32      FileSize;       // �ļ���С
    acoral_u32      FstClus;        // ��ʼ�غ�
    acoral_u32      Clus;           // ��ǰ�غ�
    acoral_u32      Offset;         // �ļ�ָ��ƫ����
} MY_FILE;


/*******************************************************************************************************/


#ifndef IN_FILE

extern void acoral_fs_init(void);

extern void FileInit(void);
/*********************************************************************************************************
** ��������: FileInit
** ��������: ��ʼ���ļ�ָ��ϵͳ
**
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern acoral_u8 RemoveFile(acoral_char *DirFileName);
/*********************************************************************************************************
** ��������: RemoveFile
** ��������: ɾ���ļ�
**
** �䡡��: DirFileName:�û�ʹ�õ��ļ���
**
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: strupr,_GetFileInfo,GetDrive,FindFDTInfo,FATDelClusChain,DelFDT
********************************************************************************************************/

extern HANDLE FileOpen(acoral_char *DirFileName, acoral_char *Type);
/*********************************************************************************************************
** ��������: FileOpen
** ��������: ��ָ����ʽ���ļ�
**
** �䡡��: DirFileName:�û�ʹ�õ��ļ���
**        Type:�򿪷�ʽ
** �䡡��: �ļ������Not_Open_FILEΪ���ܴ�
**         
** ȫ�ֱ���: ��
** ����ģ��: _FileOpenR,_FileOpenW,_FileOpenRW
********************************************************************************************************/

extern acoral_u8 FileClose(HANDLE Handle);
/*********************************************************************************************************
** ��������: FileClose
** ��������: �ر�ָ���ļ�
**
** �䡡��: Handle:�ļ����
**
** �䡡��: RETURN_OK:�ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵�� 
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern acoral_u8 FileGetCh(acoral_u8 *Ch, HANDLE Handle);
/*********************************************************************************************************
** ��������: FileGetCh
** ��������: ���ļ���һ���ֽ�
**
** �䡡��: Ch:���ض���������
**        Handle:�ļ����
** �䡡��: RETURN_OK:�ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵�� 
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern acoral_32 FileRead(void *Buf, acoral_u32 Size, HANDLE Handle);
/*********************************************************************************************************
** ��������: FileRead
** ��������: ��ȡ�ļ�
**
** �䡡��: Buf:������ص�����
**        Size:Ҫ�����ֽ���
**        Handle:�ļ����
** �䡡��: ʵ�ʶ������ֽ���
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern acoral_u8 FilePutCh(acoral_u8 Ch, HANDLE Handle);
/*********************************************************************************************************
** ��������: FilePutCh
** ��������: дһ���ֽڵ��ļ�
**
** �䡡��: Ch:Ҫд������
**        Handle:�ļ����
** �䡡��: RETURN_OK:�ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵�� 
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern acoral_32 FileWrite(const void *Buf, acoral_u32 Size, HANDLE Handle);
/*********************************************************************************************************
** ��������: FileWrite
** ��������: д�ļ�
**
** �䡡��: Buf:Ҫд������
**        Size:Ҫд���ֽ���
**        Handle:�ļ����
** �䡡��: ʵ��д���ֽ���
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern void FileCloseAll(void);
/*********************************************************************************************************
** ��������: FileCloseAll
** ��������: �ر����д򿪵��ļ�
**
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: FileInfo
** ����ģ��: AllCacheWriteBack
********************************************************************************************************/

extern acoral_u8 FileEof(HANDLE Handle);
/*********************************************************************************************************
** ��������: FileEof
** ��������: �ж��ļ��Ƿ񵽶�\д���ļ�β
**
** �䡡��: Handle:�ļ����
**
** �䡡��: 0:��
**        1:�� 
** ȫ�ֱ���: FileInfo
** ����ģ��: ��
********************************************************************************************************/

extern acoral_u8 FileSeek(HANDLE Handle, acoral_32 offset, acoral_u8 Whence);
/*********************************************************************************************************
** ��������: FileSeek
** ��������: �ƶ��ļ���\дλ��
**
** �䡡��: Handle:�ļ����
**        offset:�ƶ�ƫ����
**        Whence:�ƶ�ģʽSEEK_SET:���ļ�ͷ����SEEK_CUR:�ӵ�ǰλ�ü���SEEK_END:���ļ�β����
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

#endif


/*******************************************************************************************************/


#ifndef IN_DIR

extern acoral_u8 CurrentDrive;

extern acoral_u8 GetDrive(const acoral_char *Path);
/*********************************************************************************************************
** ��������: GetDrive
** ��������: ��ȡָ��Ŀ¼���߼��̺�
**
** �䡡��: Path:·����
**        
** �䡡��: ��������
**
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern acoral_u8 ChangeDrive(acoral_char *Drive);
/*********************************************************************************************************
** ��������: ChangeDrive
** ��������: �ı䵱ǰ�߼���
**
** �䡡��: Drive:�߼��̷��ַ���
**        
** �䡡��: RETURN_OK:�ɹ�
**        NOT_FIND_DISK:�߼��̲�����
**        PARAMETER_ERR:�Ƿ�����
** ȫ�ֱ���: ��
** ����ģ��: strupr,GetDiskInfo
********************************************************************************************************/

extern acoral_u32 GetDirClusIndex(acoral_char *Path);
/*********************************************************************************************************
** ��������: GetDirClusIndex
** ��������: ��ȡָ��Ŀ¼��ʼ�غ�
**
** �䡡��: Path:·����
**        
** �䡡��: ��ʼ�غţ�EMPTY_CLUS��Ϊ��Ŀ¼
**
** ȫ�ֱ���: ��
** ����ģ��: strupr,GetDiskInfo,FindRootFDTInfo,FindFDTInfo
********************************************************************************************************/

extern acoral_u8 MakeDir(acoral_char *Path);
/*********************************************************************************************************
** ��������: MakeDir
** ��������: ����Ŀ¼
**
** �䡡��: Path:·����
**
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDirClusIndex,strupr,ClearClus,AddRootFDT,AddFDT
********************************************************************************************************/

extern acoral_u8 RemoveDir(acoral_char *Path);
/*********************************************************************************************************
** ��������: RemoveDir
** ��������: ɾ��Ŀ¼
**
** �䡡��: Path:·����
**
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDirClusIndex,strupr,DelRootFDT,DelRootFDT
********************************************************************************************************/

extern acoral_u8 ChangeDir(acoral_char *Path);
/*********************************************************************************************************
** ��������: ChangeDir
** ��������: �ı䵱ǰĿ¼
**
** �䡡��: Path:·����
**
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDirClusIndex,GetDiskInfo
********************************************************************************************************/

#endif



/*******************************************************************************************************/




#ifndef IN_FDT

extern acoral_u8 ClearClus(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: ClearClus
** ��������: ��ָ����������������
**
** �䡡��: Disk���߼�����Ϣ
**        Index���غ�
** �䡡��: RETURN_OK:�ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,OpenSec,WriteSec,CloseSec
********************************************************************************************************/

extern acoral_u8 ChangeFDT(acoral_u8 Drive, acoral_u32 ClusIndex, FDT *FDTData);
/*********************************************************************************************************
** ��������: ChangeFDT
** ��������: �ı�ָ��Ŀ¼ָ���ļ���Ŀ¼��������
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo,SetFDTInfo
********************************************************************************************************/

extern acoral_u8  ReadFDTInfo(FDT *Rt, acoral_u8 Drive, acoral_u32 SecIndex, acoral_u16 ByteIndex);
/*********************************************************************************************************
** ��������: ReadFDTInfo
** ��������: ��ȡFDT��Ϣ
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        SecIndex��������
**        ByteIndex��ƫ����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: OpenSec,ReadSec,CloseSec
********************************************************************************************************/

extern acoral_u8  WriteFDTInfo(FDT *FDTData, acoral_u8 Drive, acoral_u32 SecIndex, acoral_u16 ByteIndex);
/*********************************************************************************************************
** ��������: WriteFDTInfo
** ��������: ��ȡFDT��Ϣ
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        SecIndex��������
**        ByteIndex��ƫ����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: OpenSec,ReadSec,CloseSec
********************************************************************************************************/

extern acoral_u8 GetFDTInfo(FDT *Rt,acoral_u8 Drive, acoral_u32 ClusIndex, acoral_u32 Index);
/*********************************************************************************************************
** ��������: GetFDTInfo
** ��������: ��ȡָ��Ŀ¼ָ���ļ���Ŀ¼����Ϣ
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        Index���ļ���Ŀ¼����FDT�е�λ��
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,ReadFDTInfo
********************************************************************************************************/

extern acoral_u8 SetFDTInfo(acoral_u8 Drive, acoral_u32 ClusIndex, acoral_u32 Index, FDT *FDTData);
/*********************************************************************************************************
** ��������: SetFDTInfo
** ��������: ��ȡָ��Ŀ¼ָ���ļ���Ŀ¼����Ϣ
**
** �䡡��: FDTData��Ҫд�����Ϣ
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        Index���ļ���Ŀ¼����FDT�е�λ��
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,WriteFDTInfo
********************************************************************************************************/

extern acoral_u8 FindFDTInfo(FDT *Rt,acoral_u8 Drive, acoral_u32 ClusIndex, acoral_char FileName[]);
/*********************************************************************************************************
** ��������: FindFDT
** ��������: ��ָ��Ŀ¼����ָ���ļ���Ŀ¼��
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo
********************************************************************************************************/

extern acoral_u8 AddFDT(acoral_u8 Drive, acoral_u32 ClusIndex, FDT *FDTData);
/*********************************************************************************************************
** ��������: AddFDT
** ��������: ��ָ��Ŀ¼������ָ���ļ���Ŀ¼��
**
** �䡡��: Rt���洢������Ϣ��ָ��
**        Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: FindFDT,GetFDTInfo,SetFDTInfo
********************************************************************************************************/

extern acoral_u8 DelFDT(acoral_u8 Drive, acoral_u32 ClusIndex, acoral_char FileName[]);
/*********************************************************************************************************
** ��������: DelFDT
** ��������: ��ָ��Ŀ¼ɾ��ָ���ļ���Ŀ¼��
**
** �䡡��: Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo,SetFDTInfo
********************************************************************************************************/

extern acoral_u8 DirIsEmpty(acoral_u8 Drive, acoral_u32 ClusIndex);
/*********************************************************************************************************
** ��������: DirIsEmpty
** ��������: 
**
** �䡡��: Drive����������
**        ClusIndex��Ŀ¼�״غ�
** �䡡��: DIR_EMPTY����
**        DIR_NOT_EMPTY������
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetFDTInfo
********************************************************************************************************/

extern acoral_u8 FDTIsLie(acoral_u8 Drive, acoral_u32 ClusIndex, acoral_char FileName[]);
/*********************************************************************************************************
** ��������: FDTIsLie
** ��������: ��ָ��Ŀ¼�鿴ָ���ļ���Ŀ¼���Ƿ����
**
** �䡡��: Drive����������
**        ClusIndex��Ŀ¼�״غ�
**        FileName���ļ���Ŀ¼����
** �䡡��: RETURN_OK���ɹ�
**        �����ο�fat.h�й��ڷ���ֵ��˵��
** ȫ�ֱ���: ��
** ����ģ��: GetDiskInfo,GetFDTInfo
********************************************************************************************************/
#endif



/*******************************************************************************************************/



#ifndef IN_DISK

extern Disk_Info DiskInfo[MAX_DRIVES];

extern void DiskInit(void);
/*********************************************************************************************************
** ��������: DiskInit
** ��������: ��ʼ���߼��̹���ģ��
**
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: DiskInfo
** ����ģ��: ��
********************************************************************************************************/

extern Disk_Info * GetDiskInfo(acoral_u8 Drive);
/*********************************************************************************************************
** ��������: GetDiskInfo
** ��������: ��ȡָ���߼�����Ϣ
**
** �䡡��: �߼��̺�
**
** �䡡��: ָ���߼�����Ϣ�Ľṹ��ָ��
**         
** ȫ�ֱ���: DiskInfo
** ����ģ��: ��
********************************************************************************************************/

//extern Disk_Info * GetEmptyDiskInfoAddr(void);
/*********************************************************************************************************
** ��������: GetEmptyDiskInfoAddr
** ��������: ��ȡ�����߼���
**
** �䡡��: ��
**
** �䡡��: ָ���߼�����Ϣ�Ľṹ��ָ��
**         
** ȫ�ֱ���: DiskInfo
** ����ģ��: ��
********************************************************************************************************/

extern acoral_32 AddFileDriver(acoral_u16  (* DiakCommand)(acoral_u8 Cammand, void *Parameter),acoral_char *devname,acoral_u8 drive);
/*********************************************************************************************************
** ��������: AddFileDriver
** ��������: ����һ���ײ���������
**
** �䡡��: DiakCommand����������ӿں���
**
** �䡡��: ��
**         
** ȫ�ֱ���: DiskInfo
** ����ģ��: ��
********************************************************************************************************/

extern void RemoveFileDriver(acoral_u8 Drive);
/*********************************************************************************************************
** ��������: RemoveFileDriver
** ��������: ɾ��һ���ײ���������
**
** �䡡��: Drive:�߼���������
**
** �䡡��: ��
**         
** ȫ�ֱ���: DiskInfo
** ����ģ��: ��
********************************************************************************************************/
#endif



/*******************************************************************************************************/



#ifndef IN_RWSEC

extern void CacheInit(void);
/*********************************************************************************************************
** ��������: CacheInit
** ��������: ��ʼ������cache
**
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: ��
********************************************************************************************************/

extern void CloseSec(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: CloseSec
** ��������: ����ָ��cache�������л����ڴ棩
**
** �䡡��: Drive���߼���������
**        Index�������������
** �䡡��: ��
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: ��
********************************************************************************************************/

extern void CacheWriteBack2(acoral_u16 Index);
/*********************************************************************************************************
** ��������: CacheWriteBack2
** ��������: ��ָ������д���߼���
**
** �䡡��: Index��cache����
**
** �䡡��: ��
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: GetDiskInfo
********************************************************************************************************/

extern void CacheWriteBack(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: CacheWriteBack
** ��������: ��ָ��cacheд���߼���
**
** �䡡��: Drive���߼���������
**        Index�������������
** �䡡��: ��
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: CacheWriteBack2
********************************************************************************************************/

extern void AllCacheWriteBack(void);
/*********************************************************************************************************
** ��������: AllCacheWriteBack
** ��������: �������Ѹı������д���߼���
**
** �䡡��: ��
**
** �䡡��: ��
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: CacheWriteBack2
********************************************************************************************************/

extern acoral_u16 GetCache(void);
/*********************************************************************************************************
** ��������: GetCache
** ��������: ��ȡһ��cache
**
** �䡡��: ��
**
** �䡡��: cache����
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: ��
********************************************************************************************************/

extern acoral_u8 *OpenSec(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: OpenSec
** ��������: Ϊ�߼����ϵ�һ��������һ��cache������
**
** �䡡��: Drive�����̺�
**        Index��������
** �䡡��: ָ��ָ���������ݵ�ָ��
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: ��
********************************************************************************************************/

extern acoral_u8 ReadSec(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: ReadSec
** ��������: ���߼��̶�����
**
** �䡡��: Disk���߼�����Ϣ
**        Index��������
** �䡡��: TRUE:�ɹ�
**         FALSE:ʧ��
** ȫ�ֱ���: DiskCache
** ����ģ��: GetDiskInfo
********************************************************************************************************/

extern void WriteSec(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: WriteSec
** ��������: ˵��ָ���߼��̵�ָ��һ����������д
**
** �䡡��: Disk���߼�����Ϣ
**        Index��������
** �䡡��: ��
**         
** ȫ�ֱ���: DiskCache
** ����ģ��: ��
********************************************************************************************************/

#endif




/*******************************************************************************************************/




#ifndef IN_FAT

extern acoral_u32 FATGetNextClus(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: FATGetNextClus
** ��������: ����FAT��ָ���ص���һ���غ�
**
** �䡡��: Drive����������
**        Index���غ�
** �䡡��: ��һ���غ�
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

void FATSetNextClus(acoral_u8 Drive, acoral_u32 Index, acoral_u32 Next);
/*********************************************************************************************************
** ��������: FATSetNextClus
** ��������: ɾ��ָ������
**
** �䡡��: Drive����������
**        Index���������״غ�
**        Next����һ���غ�
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/


extern acoral_u32 FATAddClus(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: FATAddClus
** ��������: Ϊָ����������һ����
**
** �䡡��: Drive����������
**        Index������������һ���غţ����Ϊ0����Ϊһ����������һ����
** �䡡��: ���ӵĴغ�
**         
** ȫ�ֱ���: ��
** ����ģ��: ��
********************************************************************************************************/

extern void FATDelClusChain(acoral_u8 Drive, acoral_u32 Index);
/*********************************************************************************************************
** ��������: FATDelClusChain
** ��������: ɾ��ָ������
**
** �䡡��: Drive����������
**        Index���������״غ�
** �䡡��: ��
**         
** ȫ�ֱ���: ��
** ����ģ��: FATGetNextClus
********************************************************************************************************/
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
#endif