@ECHO OFF
REM ************************************************************************
REM make-fzpz              ������ ��������� *.fzpz ��� ����㧪� �� Fritzing
REM Authors: Mike Pilgrem / Vladimir Trufanov
REM Remark:  ⠪ ��� ����� bat-䠩��� �室�� � MS-DOS, � ����� 䠩�
REM          ������ ���� ��࠭� � ����஢�� OEM Russian (866)
REM ************************************************************************

REM ����� ������� ������ �� ࠡ�� � ���-䠩��� (�� ᯮᮡ�):
REM   make-file /?
REM   make-file -h
REM   make-file --help

REM ��� ᡮન ��������� ᫥��� � ����⢥ ��ࠬ��� 㪠���� ��� ���������
REM   make-file detail

REM �᫨ �ॡ���� ������, �室�� �� �뢮� ⥪�� �����
IF "%1" == "/?" GOTO HELP
IF "%1" == "-h" GOTO HELP
IF "%1" == "--help" GOTO HELP

REM �᫨ � �����⠫��� "part" 䠩� ��⠤����� ��⠫� "detail.fzp"
REM ��������� ��� ��������� ��⠫��,
REM � �뤠�� �।�०����� � �����蠥� ࠡ���

IF NOT EXIST part\%1.fzp (
    ECHO ���� ��⠤����� ��⠫� part\%1.fzp �� ������.
    GOTO END
)

REM �᫨ � �����⠫��� " svg\icon" 䠩� ������ ��⠫� "detail_icon.svg"
REM ���������, � �뤠�� �।�०����� � �����蠥� ࠡ���

IF NOT EXIST svg\icon\%1_icon.svg (
    ECHO �������� 䠩� ������ ��⠫�: svg\icon\%1_icon.svg, �� �� ������
    GOTO END
)

REM �᫨ � �����⠫��� " svg\breadboard" ��������� 䠩� ����⭮��
REM �।�⠢����� ��⠫� "detail_breadboard.svg",
REM � �뤠�� �।�०����� � �����蠥� ࠡ���

IF NOT EXIST svg\breadboard\%1_breadboard.svg (
    ECHO �� ������ 䠩� ����⭮�� �।�⠢����� ��⠫�:
    ECHO svg\breadboard\%1_breadboard.svg.
    GOTO END
)

REM �஢��塞 �।�⠢����� ��⠫� � ���஭��� �奬�

IF NOT EXIST svg\schematic\%1_schematic.svg (
    ECHO �� ������ 䠩� �।�⠢����� ��⠫� � ���஭��� �奬�:
    ECHO svg\schematic\%1_schematic.svg.
    GOTO END
)

REM �஢��塞 �।�⠢����� ��⠫� �� ���⭮� ����

IF NOT EXIST svg\pcb\%1_pcb.svg (
    ECHO �� ������ 䠩� �।�⠢����� ��⠫� �� ���⭮� ����:
    ECHO svg\pcb\%1_pcb.svg.
    GOTO END
)

REM �।�०���� � ����稨 �०��� ���ᨨ 䠩�� ��������� "detail.fzpz"
REM � 㤠�塞 ��� �� ���⢥ত���� �����

IF EXIST %1.fzpz (
    ECHO ���� ��⠫� %1.fzpz 㦥 �������.
    ECHO �� �㤥� 㤠���, �᫨ �� �த�����.
    CHOICE /N /M "�� ��� �த������ (Y/N)?"
    IF ERRORLEVEL 2 GOTO END
    DEL %1.fzpz
)

REM ������ ����� 䠩��� ��� ᪫��뢠��� �� � ��娢 ��⠫�

COPY part\%1.fzp part.%1.fzp >NUL
COPY svg\icon\%1_icon.svg svg.icon.%1_icon.svg >NUL
COPY svg\breadboard\%1_breadboard.svg svg.breadboard.%1_breadboard.svg >NUL
COPY svg\schematic\%1_schematic.svg svg.schematic.%1_schematic.svg >NUL
COPY svg\pcb\%1_pcb.svg svg.pcb.%1_pcb.svg >NUL

REM �����뢠�� 䠩�� � zip-��娢 ��⠫� "detail.fzpz"
7z a -tzip %1.fzpz part.%1.fzp svg.icon.%1_icon.svg svg.breadboard.%1_breadboard.svg svg.schematic.%1_schematic.svg svg.pcb.%1_pcb.svg >NUL

REM ����塞 �����
DEL part.%1.fzp svg.icon.%1_icon.svg svg.breadboard.%1_breadboard.svg svg.schematic.%1_schematic.svg svg.pcb.%1_pcb.svg

REM ����頥�, �� 䠩� ��⠫� ᮧ���
ECHO ���� 䠩� ��⠫� ��� Fritzing: %1.fzpz ᮧ���.

GOTO END

:HELP
ECHO ��� ᮧ����� ��������� Fritzing �ॡ���� ����稥 ��娢��� 7z.exe �
ECHO ⥪�饬 ��⠫��� ��� ������ ���� �஫���� ���� � ��� �ᯮ������� � Windows.
ECHO.
ECHO.  make-fzpz [/?^|-h^|--help] ^<partname^>
ECHO.
ECHO ^<partname^>       ��� ��⠫� �� Fritzing. �� ������ ������� �஡����.
ECHO [/?^|-h^|--help]   �뢮��� �� ᮮ�饭�� �����.
ECHO.
ECHO �।����������, �� *.fzp � *.svg 䠩�� ࠧ��饭� � ��⠫����
ECHO \part, \svg\icon, \svg\breadboard, \svg\schematic and \svg\pcb, � �������
ECHO ^<partname^>.fzp
ECHO ^<partname^>_^<view^>.svg
ECHO.
ECHO ��� ^<view^> ���� �� ����� �।�⠢�����: icon, breadboard, schematic or pcb.
ECHO.
:END

REM ------------------------------------------------------------------------
REM Copyright (c) 2015, Mike Pilgrem
REM All rights reserved.

REM Redistribution and use in source and binary forms, with or without
REM modification, are permitted provided that the following conditions are met:

REM 1. Redistributions of source code must retain the above copyright notice,
REM    this list of conditions and the following disclaimer.

REM 2. Redistributions in binary form must reproduce the above copyright notice,
REM    this list of conditions and the following disclaimer in the documentation
REM    and/or other materials provided with the distribution.

REM THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
REM AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
REM IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
REM ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
REM LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
REM CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
REM SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
REM INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
REM CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
REM ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
REM POSSIBILITY OF SUCH DAMAGE.
REM ------------------------------------------------------------------------
