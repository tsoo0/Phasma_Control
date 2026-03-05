function [t,data]=get_data_joerger(shot,varargin)
host=getenv('PHASMA_HOST');
mdsinfo=mdsInfo();
if length(varargin)==0 | (length(varargin)==1 & strcmp(varargin{1},'server'))
    if mdsinfo.isConnected==false
        %ret_con=mdsconnect(host);
        ret_con=mdsconnect('157.182.26.6:57800');
    else
        ret_con=1;
    end
    ret_open=mdsopen('phasma',shot);
    if mod(ret_con,2)==1 & ret_open==shot
        t=double((mdsvalue('dim_of(\JOERGER_01_1)')));
        for index_ch=1:8
            data(:,index_ch)=double((mdsvalue(['\JOERGER_01_',num2str(index_ch)]))');
        end
    else
        t=[];data=[];
    end
    mdsclose();
%     mdsdisconnect();
end

if length(varargin)==1 & strcmp(varargin{1},'local')
    folderpath='/Users/peiyunshi/Dropbox/Lab_Rountine/MatLab/DATA/'; %Mac - Dropbox
    temp=dir([folderpath,num2str(shot),'_JOERGER*.txt']);

    if length(temp)==0
        folderpath='/Users/peiyunshi/OneDrive - West Virginia University/DATA/'; %Mac - OneDrive
        temp=dir([folderpath,num2str(shot),'_JOERGER*.txt']);
    end

    if length(temp)==0
        folderpath='\Users\ps00003\Dropbox\Lab_Rountine\MatLab\DATA\'; %Office Room - Dropbox
        temp=dir([folderpath,num2str(shot),'_JOERGER*.txt']);
    end

    if length(temp)==0
        folderpath='C:\Users\ps00003\OneDrive - West Virginia University 1\DATA\'; %Office Room - OneDrive
        temp=dir([folderpath,num2str(shot),'_JOERGER*.txt']);
    end

    if length(temp)==0
        folderpath='D:\MDSPLUS_PHASMA\RAWDATA\'; %Lab
%         folderpath='C:\Users\Scime Lab\Documents\LabVIEW Projects\DATA\'; %Lab
        temp=dir([folderpath,num2str(shot),'_JOERGER*.txt']);
    end
    if length(temp)~=0
        fid=fopen([folderpath,temp(1).name]);

        rawdata=fscanf(fid,'%f %f %f %f %f %f %f %f %f',[9,inf]);
        % rawdata=reshape(rawdata,size(rawdata,2),size(rawdata,1));
        t=double((rawdata(1,:))');
        data=double((rawdata(2:9,:))');
        fclose(fid)
    else
        t=[];data=[];
    end
end