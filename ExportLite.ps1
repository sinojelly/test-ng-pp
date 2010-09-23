# export mockcpp without boost

$env:path=$env:path+";D:\Tools\CMD\SubversionClient"

$dest_dir="D:\testngpp_exported_new"

function export_subdir($subdir) 
{
	if (!(test-path $dest_dir\$subdir)) { mkdir $dest_dir\$subdir }
	svn export $subdir $dest_dir\$subdir --force	
}

export_subdir 3rdparty\msinttypes
export_subdir 3rdparty\mem_checker
export_subdir include
export_subdir samples
export_subdir scripts
export_subdir src
export_subdir tests

svn export --depth=files . $dest_dir --force



