//
//  THFTPAPI.h
//  MyFTP
//
//  Created by TanHao on 13-6-6.
//  Copyright (c) 2013年 http://www.tanhao.me. All rights reserved.
//

//链接服务器
int ftp_connect( char *host, int port, char *user, char *pwd );
//断开服务器
int ftp_quit( int c_sock);

//设置表示类型
int ftp_type( int c_sock, char mode );

//改变工作目录
int ftp_cwd( int c_sock, char *path );
//回到上一层目录
int ftp_cdup( int c_sock );
//创建目录
int ftp_mkd( int c_sock, char *path );
//列表
int ftp_list( int c_sock, char *path, void **data, unsigned long long *data_len);

//下载文件
int ftp_retrfile( int c_sock, char *s, char *d ,unsigned long long *stor_size, int *stop);
//上传文件
int ftp_storfile( int c_sock, char *s, char *d ,unsigned long long *stor_size, int *stop);

//修改文件名&移动目录
int ftp_renamefile( int c_sock, char *s, char *d );
//删除文件
int ftp_deletefile( int c_sock, char *s );
//删除目录
int ftp_deletefolder( int c_sock, char *s );
