function out = file_write_char(data,file_path)

fid = fopen(file_path,'w');
if fid == -1
    out = -1;
    error('can not open the file');
end


fwrite(fid,data);
fclose(fid);
out = 1;
end