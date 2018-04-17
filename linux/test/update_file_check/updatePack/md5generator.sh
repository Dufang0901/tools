#! /bin/sh
echo "Begin to generate md5 files"
ROOT_DIR="/mnt/hgfs/svn/duheng/test/update_file_check/updatePack"
ROUTEALL_DIR="$ROOT_DIR/route_all/"
BICCODE_DIR="$ROOT_DIR/BicCode/"
CFGFILE_DIR="$ROOT_DIR/BicCfgFile/"
ROUTEFS_DIR="$ROOT_DIR/BicRootfs/"
APCODE_DIR="$ROOT_DIR/ApCode/"

#生成线路文件MD5校验文件
if [ -f route_all.zip ]
then
	unzip -o route_all.zip -d .
	sync
fi

if [ -d route_all ]
then
	cd route_all
	find ./ -type f -print|xargs md5sum|sort -u > ../route_all.md5
	sync
	echo "route_all MD5 file generated successfully"
	cd ../
	rm -rf route_all
	sync
else
	echo "route_all MD5 file generating failed"
fi



#生成应用程序文件MD5校验文件
if [ -f BicCode.zip ]
then
	unzip -o BicCode.zip -d .
	sync
fi

if [ -d BicCode ]
then
	cd BicCode
	find ./ -type f -print|xargs md5sum|sort -u > ../BicCode.md5
	sync
	echo "BicCode MD5 file generated successfully"
	cd ../
	rm -rf BicCode
	sync
else
	echo "BicCode MD5 file generating failed"
fi



#生成配置文件MD5校验文件
if [ -f BicCfgFile.zip ]
then
	unzip -o BicCfgFile.zip -d .
	sync
fi

if [ -d BicCfgFile ]
then
	cd BicCfgFile
	find ./ -type f -print|xargs md5sum|sort -u > ../BicCfgFile.md5
	sync
	echo "BicCfgFile MD5 file generated successfully"
	cd ../
	rm -rf BicCfgFile
	sync
else
	echo "BicCfgFile MD5 file generating failed"
fi


#生成根文件系统文件MD5校验文件
if [ -f BicRootfs.zip ]
then
	unzip -o BicRootfs.zip -d .
	sync
fi

if [ -d BicRootfs ]
then
	cd BicRootfs
	find ./ -type f -print|xargs md5sum|sort -u > ../BicRootfs.md5
	sync
	echo "BicRootfs MD5 file generated successfully"
	cd ../
	rm -rf BicRootfs
	sync
else
	echo "BicRootfs MD5 file generating failed"
fi


#生成AP固件文件MD5校验文件
if [ -f ApCode.zip ]
then
	unzip -o ApCode.zip -d .
	sync
fi

if [ -d ApCode ]
then
	cd ApCode
	find ./ -type f -print|xargs md5sum|sort -u > ../ApCode.md5
	sync
	echo "ApCode MD5 file generated successfully"
	cd ../
	rm -rf ApCode
	sync
else
	echo "ApCode MD5 file generating failed"
fi
