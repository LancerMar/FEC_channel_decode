function out = file_read_double(file_path)
fid = fopen(file_path);
if fid == -1
    error('can not open the file');
end

% 读取数据
out = fread(fid, inf, 'double');
% 关闭文件
fclose(fid);
end