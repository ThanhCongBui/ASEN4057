function varargout = MATLABGUI(varargin)
% MATLABGUI MATLAB code for MATLABGUI.fig
%      MATLABGUI, by itself, creates a new MATLABGUI or raises the existing
%      singleton*.
%
%      H = MATLABGUI returns the handle to a new MATLABGUI or the handle to
%      the existing singleton*.
%
%      MATLABGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MATLABGUI.M with the given input arguments.
%
%      MATLABGUI('Property','Value',...) creates a new MATLABGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before MATLABGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to MATLABGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help MATLABGUI

% Last Modified by GUIDE v2.5 17-Feb-2017 17:07:06

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @MATLABGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @MATLABGUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before MATLABGUI is made visible.
function MATLABGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to MATLABGUI (see VARARGIN)
handles.prn = 0; 
handles.msec_to_process = 0; 
handles.fs = 0; 
handles.intfreq = 0;
handles.msectoavg = 0;
set(handles.animButton,'Enable','off')
% Choose default command line output for MATLABGUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes MATLABGUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = MATLABGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in ProcessButton.
function ProcessButton_Callback(hObject, eventdata, handles)
% hObject    handle to ProcessButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if (handles.prn == 0) || (handles.msec_to_process == 0) || (handles.fs == 0) 
    disp('You forgot to set the parameters. Please go to the input panel and set them.')
else
disp('GPS Bistatic Processing')
%%input parameters to set (specific to data file)
prn = handles.prn;  %GPS satellite number (03 05 06 09 12 14 18 21 22 30 31 )
msec_to_process = handles.msec_to_process;  %msec of data to process
fs = handles.fs;  %sampling frequency
intfreq = handles.intfreq;  %intermediate frequency
corrspacing=-28.1:0.05:1.1;  %correlator measurement spacing
%the following are now hard coded along with the complex data format
%filename='GPSantennaUp.sim';  %direct file
%filename2='GPSantennaDown.sim';  %reflected file

%%first open the direct data file and get first block of data to work with
fid=fopen('GPSantennaUp.sim','rb');  %open binary file containing direct data
rawdat=(fread(fid,2000000,'schar'))';  %read in 2M samples
rawdat=rawdat(1:2:end)+ i .* rawdat(2:2:end);  %convert to complex IQ pairs
fclose(fid);  %close the file

%%first do satellite acqusition and get approx code phase/freq estimate
disp('Initiating GPS satellite acquisition...')
[fq_est,cp_est,c_meas,testmat]=acq4(prn,intfreq,9000,rawdat,fs,4,0);
disp(['  Acquisition Metric is: ',num2str(c_meas(1)),' (should be >2.0)']); 
%refine the acqusition estimate to get better code phase/frequency
[fq_est,cp_est,c_meas,testmat]=acq4(prn,fq_est,400,rawdat,fs,10,0);
disp(['  Refined Acq Metric is: ',num2str(c_meas(1)),' (should be >2.0)']); 

%%now do the main bistatic tracking/processing
disp('Initiating GPS satellite signal tracking...')
tstart=tic;  %start the timer
[e_i,e_q,p_i,p_q,l_i,l_q,carrierfq,codefq] = ...
    gpstrackcorr(prn, cp_est, fq_est, fs, corrspacing, msec_to_process);
toc(tstart);
%%Simple script to plot the results of the bistatic processing of the
%direct channel;  create two figures and plot important aspects

axes(handles.correlateAxes)
plot(p_i .^2 + p_q .^ 2, 'g.-')
hold on
grid
plot(e_i .^2 + e_q .^ 2, 'bx-')
plot(l_i .^2 + l_q .^ 2, 'r+-')
hold off
xlabel('milliseconds')
ylabel('amplitude')
title('Correlation Results')
legend('prompt','early','late')
axes(handles.promptiAxes);
plot(p_i)
grid
xlabel('milliseconds')
ylabel('amplitude')
title('Prompt I Channel')
axes(handles.promptqAxes);
plot(p_q)
grid
xlabel('milliseconds')
ylabel('amplitude')
title('Prompt Q Channel')

axes(handles.trackedcodeAxes);
plot(1.023e6 - codefq)
grid
xlabel('milliseconds')
ylabel('Hz')
title('Tracked Code Frequency (Deviation from 1.023MHz)')
axes(handles.trackedintAxes);
plot(carrierfq)
grid
xlabel('milliseconds')
ylabel('Hz')
title('Tracked Intermediate Frequency')

%%finished!
disp('Finished!')%  Run scripts: corrplot.m or plotresults.m for results visualization')
set(handles.animButton,'Enable','on')
if handles.intfreq == 0
    set(handles.IntermediateFrequencyBox,'String','0'); 
end
end


function SatelliteNumberBox_Callback(hObject, eventdata, handles)
% hObject    handle to SatelliteNumberBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.prn = str2double(get(hObject,'String'));
guidata(hObject, handles);

% Hints: get(hObject,'String') returns contents of SatelliteNumberBox as text
%        str2double(get(hObject,'String')) returns contents of SatelliteNumberBox as a double


% --- Executes during object creation, after setting all properties.
function SatelliteNumberBox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to SatelliteNumberBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function SamplingFrequencyBox_Callback(hObject, eventdata, handles)
% hObject    handle to SamplingFrequencyBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.fs = str2double(get(hObject,'String'));
guidata(hObject, handles);

% Hints: get(hObject,'String') returns contents of SamplingFrequencyBox as text
%        str2double(get(hObject,'String')) returns contents of SamplingFrequencyBox as a double


% --- Executes during object creation, after setting all properties.
function SamplingFrequencyBox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to SamplingFrequencyBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function IntermediateFrequencyBox_Callback(hObject, eventdata, handles)
% hObject    handle to IntermediateFrequencyBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.intfreq = str2double(get(hObject,'String'));
guidata(hObject, handles);
% Hints: get(hObject,'String') returns contents of IntermediateFrequencyBox as text
%        str2double(get(hObject,'String')) returns contents of IntermediateFrequencyBox as a double



% --- Executes during object creation, after setting all properties.
function IntermediateFrequencyBox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to IntermediateFrequencyBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function msectoprocessBox_Callback(hObject, eventdata, handles)
% hObject    handle to msectoprocessBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.msec_to_process = str2double(get(hObject,'String'));
guidata(hObject, handles);
% Hints: get(hObject,'String') returns contents of msectoprocessBox as text
%        str2double(get(hObject,'String')) returns contents of msectoprocessBox as a double


% --- Executes during object creation, after setting all properties.
function msectoprocessBox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to msectoprocessBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function msectoavgBox_Callback(hObject, eventdata, handles)
% hObject    handle to msectoavgBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.msectoavg = str2double(get(hObject,'String'));
guidata(hObject, handles);
% Hints: get(hObject,'String') returns contents of msectoavgBox as text
%        str2double(get(hObject,'String')) returns contents of msectoavgBox as a double


% --- Executes during object creation, after setting all properties.
function msectoavgBox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to msectoavgBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in animButton.
function animButton_Callback(hObject, eventdata, handles)
% hObject    handle to animButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
%This script plots the important values in the GPS Bistatic processing 
%for the direct and reflected channels

%close all; clear; clc
if handles.msectoavg == 0
    disp('You forgot to set the value "Milliseconds to Average". Please go back and set it.')
else
%reflected spacings  (same as those used in processing)
spacing=-28.1:0.05:1.1; %recognize these spacings are reversed
[a,b]=size(spacing);
spacing2=spacing(find(spacing==(-spacing(end))):end);
[aa,bb]=size(spacing2);

%set filenames
fid1=fopen('OutDI.bin','rb');
fid2=fopen('OutDQ.bin','rb');
fid3=fopen('OutRI.bin','rb');
fid4=fopen('OutRQ.bin','rb');

%set avging - typical value is 200msec
disp('BiStatic Radar Processing')
avgnum = handles.msectoavg; %input('How Many Msec should be averaged : ');

%get the first set of data to initiate loop
contloop=1;
[corri,cnt]=fread(fid1,avgnum*bb,'float');
if (cnt ~= avgnum*bb)
    fclose all
    disp('got all the data possible corr_dir_in')
    contloop=0;
end
[corrq,cnt]=fread(fid2,avgnum*bb,'float');
if (cnt ~= avgnum*bb)
    fclose all
    disp('got all the data possible corr_dir_qd')
    contloop=0;
end
[corrii,cnt]=fread(fid3,avgnum*b,'float');
if (cnt ~= avgnum*b)
    fclose all
    disp('got all the data possible corr_rfct_in')
    contloop=0;
end
[corrqq,cnt]=fread(fid4,avgnum*b,'float');
if (cnt ~= avgnum*b)
    fclose all
    disp('got all the data possible corr_rfct_in')
    contloop=0;
end

loopcnt=1;
while (contloop==1) %infinite loop to continue while data in the files
    
    corri=reshape(corri,bb,avgnum);
    corrq=reshape(corrq,bb,avgnum);
    corrii=reshape(corrii,b,avgnum);
    corrqq=reshape(corrqq,b,avgnum);
    
    %----------------------------------------------------------------------
    
    coherent=0;  %set equal to 1 for coherent, or 0 for noncoherent
    if (coherent)
        for inda = 1:avgnum
            [signmax,signind]=max(abs(corri(:,inda)));
            if (corri(signind,inda) >= 0)
                %no need to do anything as already post
            else
                %flip sign to make it pos
                corri(:,inda) = -1 .* corri(:,inda);
                corrq(:,inda) = -1 .* corrq(:,inda);
                corrii(:,inda) = -1 .* corrii(:,inda);
                corrqq(:,inda) = -1 .* corrqq(:,inda);
            end
        end
        di2=sum((corri'));
        dq2=sum((corrq'));
        ri2=sum((corrii'));
        rq2=sum((corrqq'));
    else
        di2=sum(abs(corri'));
        dq2=sum(abs(corrq'));
        ri2=sum(abs(corrii'));
        rq2=sum(abs(corrqq'));
    end
    %----------------------------------------------------------------------
    
    %do the averaging
    %reorder
    di2=di2(bb:-1:1);
    dq2=dq2(bb:-1:1);
    d3=sqrt(di2 .* di2 + dq2 .* dq2);
    d3save(loopcnt,:)=d3;
    
    %find max and normalize
    ddmax=max(d3);
    di2=di2 ./ ddmax;
    dq2 = dq2 ./ ddmax;
    d3 = d3 ./ ddmax;
    
    %do avg
    %reorder
    ri2=ri2(b:-1:1);
    rq2=rq2(b:-1:1);
    r3=sqrt(ri2 .* ri2 + rq2 .* rq2);
    r3save(loopcnt,:)=r3;
    
    %find max and normalize
    rrmax=max(r3); %use for normalization
    [rrmaxrat,jj]=max(r3(66:b)); %use for max ratio
    ii=jj+55;
    ri2=ri2 ./ rrmax;
    rq2 = rq2 ./ rrmax;
    r3 = r3 ./ rrmax;
    
    %plot correlation results
%     figure(100)
    
    %direct signal
    dmin=min([di2 dq2]);
    d3min=min([d3]);
    axes(handles.directiAxes);
    plot(spacing2,di2,'r.');hold on
    plot(spacing2,di2,'c');hold off
    axis([(spacing2(1)-0.1) (spacing2(end)+0.1) (dmin-0.05) 1.05])
    title('direct (I)')
    axes(handles.directqAxes);
    plot(spacing2,dq2,'g.');hold on
    plot(spacing2,dq2,'c');hold off
    axis([(spacing2(1)-0.1) (spacing2(end)+0.1) (dmin-0.05) 1.05])
    title('direct (Q)')
    axes(handles.reflectdirectAxes);
    plot(spacing2,d3,'b.');hold on
    plot(spacing2,d3,'m');hold off
    axis([(spacing2(1)-0.1) (spacing2(end)+0.1) (d3min-0.05) 1.05])
    powerrat(loopcnt) = rrmaxrat / ddmax;
    
    %reflected signal
    title(['reflected/direct ratio: ',num2str(powerrat(loopcnt))])
    rmin=min([ri2 rq2]);
    r3min=min([r3]);
    axes(handles.reflectiAxes);
    plot(-spacing(end:-1:1),ri2,'r.');hold on
    plot(-spacing(end:-1:1),ri2,'c');hold off
    axis([(-spacing(end)-0.1) (-spacing(1)+0.1) (rmin-0.05) 1.05])
    title('reflect (I)')
    axes(handles.reflectqAxes);
    plot(-spacing(end:-1:1),rq2,'g.');hold on
    plot(-spacing(end:-1:1),rq2,'c');hold off
    axis([(-spacing(end)-0.1) (-spacing(1)+0.1)  (rmin-0.05) 1.05])
    title('reflect (Q)')
    axes(handles.reflectcompAxes);
    plot(-spacing(end:-1:1),r3,'b.');hold on
    plot(-spacing(end:-1:1),r3,'m');hold off
    axis([(-spacing(end)-0.1) (-spacing(1)+0.1)  (r3min-0.05) 1.05])
    
    %set lower bound on where max can be, in case of direct creeping in
    spthresh=find(spacing > 0.75);
    lwbnd=spthresh(1);
    upbnd=b;
    pathdelay(loopcnt)=293*ii*0.05;
    title(['reflected composite; path delay of ',num2str(pathdelay(loopcnt),6),'m '])
    xlabel(['time elapsed : ',num2str(loopcnt*avgnum/(60*1000),4),' min; Int#:',int2str(loopcnt)])
    
    [corri,cnt]=fread(fid1,avgnum*bb,'float');
    if (cnt ~= avgnum*bb)
        disp('  got all the data possible corr_dir_in')
        contloop=0;
    end
    [corrq,cnt]=fread(fid2,avgnum*bb,'float');
    if (cnt ~= avgnum*bb)
        disp('  got all the data possible corr_dir_qd')
        contloop=0;
    end
    [corrii,cnt]=fread(fid3,avgnum*b,'float');
    if (cnt ~= avgnum*b)
        disp('  got all the data possible corr_rfct_in')
        contloop=0;
    end
    [corrqq,cnt]=fread(fid4,avgnum*b,'float');
    if (cnt ~= avgnum*b)
        disp('  got all the data possible corr_rfct_in')
        contloop=0;
    end
    loopcnt = loopcnt +1 ;
    
    pause(0.001)
end
loopcnt=loopcnt-1;  %adjust for not doing one last iteration

%draw fig of path delay as function of time
figure(200)
subplot(2,1,1),plot((1:loopcnt)*(avgnum/1000),pathdelay,'c');hold on
subplot(2,1,1),plot((1:loopcnt)*(avgnum/1000),pathdelay,'c.');
title(['Estimated Path Delay for Reflected Signal (',int2str(avgnum),' msec avg)'])
ylabel('meters')
xlabel('seconds')
grid
subplot(2,1,2),plot((1:loopcnt)*(avgnum/1000),powerrat,'b.');hold on
subplot(2,1,2),plot((1:loopcnt)*(avgnum/1000),powerrat,'g');hold off
title(['Amplitude Ratio ((reflected max)/(direct max))'])
ylabel('ratio')
xlabel('seconds')
grid
fclose all;
end