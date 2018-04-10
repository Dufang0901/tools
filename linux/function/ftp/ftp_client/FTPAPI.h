//
//  THFTPAPI.h
//  MyFTP
//
//  Created by TanHao on 13-6-6.
//  Copyright (c) 2013�� http://www.tanhao.me. All rights reserved.
//

//���ӷ�����
int ftp_connect( char *host, int port, char *user, char *pwd );
//�Ͽ�������
int ftp_quit( int c_sock);

//���ñ�ʾ����
int ftp_type( int c_sock, char mode );

//�ı乤��Ŀ¼
int ftp_cwd( int c_sock, char *path );
//�ص���һ��Ŀ¼
int ftp_cdup( int c_sock );
//����Ŀ¼
int ftp_mkd( int c_sock, char *path );
//�б�
int ftp_list( int c_sock, char *path, void **data, unsigned long long *data_len);

//�����ļ�
int ftp_retrfile( int c_sock, char *s, char *d ,unsigned long long *stor_size, int *stop);
//�ϴ��ļ�
int ftp_storfile( int c_sock, char *s, char *d ,unsigned long long *stor_size, int *stop);

//�޸��ļ���&�ƶ�Ŀ¼
int ftp_renamefile( int c_sock, char *s, char *d );
//ɾ���ļ�
int ftp_deletefile( int c_sock, char *s );
//ɾ��Ŀ¼
int ftp_deletefolder( int c_sock, char *s );
