\ProvidesClass{ICS_thesis}[2016/04/05 class for ICS slides]
\typeout{-- 2017/02/28 revision V0.2 by W.Zhiliang}
\typeout{-- This is early beta.}
\NeedsTeXFormat{LaTeX2e}

\newif\if@langGer      \global\@langGertrue
\newif\if@typeFinal      \global\@typeFinaltrue

% define style input options
\DeclareOption{LSR}{\def\@institute{LSR}}%
\DeclareOption{ITR}{\def\@institute{ITR}}%
\DeclareOption{ICS}{\def\@institute{ICS}}%

\DeclareOption{german}{\@langGertrue}
\DeclareOption{english}{\@langGerfalse}

\DeclareOption{final}{\@typeFinaltrue}
\DeclareOption{intermediate}{\@typeFinalfalse}

\DeclareOption{BA}{\def\@worktype{BA}}%
\DeclareOption{DA}{\def\@worktype{DA}}%
\DeclareOption{MA}{\def\@worktype{MA}}%
\DeclareOption{HS}{\def\@worktype{HS}}%
\DeclareOption{PP}{\def\@worktype{PP}}%
\DeclareOption{SeA}{\def\@worktype{SeA}}%
\DeclareOption{IP}{\def\@worktype{IP}}%
\DeclareOption{FP}{\def\@worktype{FP}}%


% default to german
\ExecuteOptions{german}
% default to final submission
\ExecuteOptions{final}
% make it into effect
\ProcessOptions

% so far only standard format
\LoadClass[a4paper,twoside, openright,12pt]{report}

% load packages
\RequirePackage{subfigure}
\RequirePackage{verbatim}
\RequirePackage{psfrag,amsmath,amssymb,amsbsy,graphics,float}
\RequirePackage{graphicx, color, soul}
\RequirePackage{ifthen}
\if@langGer
  \RequirePackage[ngerman]{babel}
  \RequirePackage[utf8]{inputenc}
\else
  \RequirePackage[latin1]{inputenc}
\fi

% some helper variables for \ifx
\def\@worktypeDA{DA}
\def\@worktypeBA{BA}
\def\@worktypeMA{MA}
\def\@worktypeHS{HS}
\def\@worktypePP{PP}
\def\@worktypeSeA{SeA}
\def\@worktypeIP{IP}
\def\@worktypeFP{FP}
\def\@instituteLSR{LSR}
\def\@instituteITR{ITR}
\def\@instituteICS{ICS}

\newdimen\LSRx
\newdimen\LSRzero
\newdimen\LSRborder
\LSRborder 6mm
\LSRx 10mm
\LSRzero 10pt


%_______Kopf- und Fußzeile_______________________________________________________
\RequirePackage{fancyhdr}
\pagestyle{fancy}
%um Kopf- und Fußzeile bei chapter-Seiten zu reaktivieren
\newcommand{\helv}{%
   \fontfamily{phv}\fontseries{a}\fontsize{9}{11}\selectfont}
\fancypagestyle{plain}{	
	\fancyfoot{}% keine Fußzeile
	\fancyhead[RE]{\helv\leftmark}% Rechts auf geraden Seiten=innen; in \leftmark stehen \chapters
	\fancyhead[LO]{\helv\rightmark}% Links auf ungeraden Seiten=außen;in \rightmark stehen \sections
	\fancyhead[RO,LE]{\thepage}}%Rechts auf ungeraden und links auf geraden Seiten
%Kopf- und Fußzeile für alle anderen Seiten
\fancyfoot{}
\fancyhead[RE]{\helv\leftmark}
\fancyhead[LO]{\helv\rightmark}%alt:\fancyhead[LO]{\itshape\rightmark}
\fancyhead[RO,LE]{\thepage}
%________________________________________________________________________________


%_Definieren der Ränder und Längen__________
\setlength{\textwidth}{15cm}
\setlength{\textheight}{22cm}
\setlength{\evensidemargin}{-2mm}
\setlength{\oddsidemargin}{11mm}
\setlength{\headwidth}{15cm}
\setlength{\topmargin}{10mm}
\setlength{\parindent}{0pt} % Kein Einrücken beim Absatz!!
%___________________________________________

%_Hyperref for CC Url__________
\RequirePackage{hyperref}
%___________________________________________


%_________select correct heading________________
\if@langGer
  \typeout{-- language set to german.}
  \ifx\@worktype\@worktypeDA
    \def\@worktypetext{DIPLOMARBEIT}
    \typeout{-- Type: Diplomarbeit.}
  \else
  \ifx\@worktype\@worktypeBA
    \def\@worktypetext{BACHELORARBEIT}
    \typeout{-- Type: Bachelorarbeit.}
  \else
  \ifx\@worktype\@worktypeMA
    \def\@worktypetext{MASTERARBEIT}
    \typeout{-- Type: Masterarbeit.}
  \else
  \ifx\@worktype\@worktypeHS
    \def\@worktypetext{HAUPTSEMINAR}
    \typeout{-- Type: Hauptseminar.}
  \else
  \ifx\@worktype\@worktypePP
    \def\@worktypetext{PROJEKTPRAKTIKUM}
    \typeout{-- Type: Projektpraktikum.}
  \else
  \ifx\@worktype\@worktypeSeA
    \def\@worktypetext{SEMESTERARBEIT}
    \typeout{-- Type: Semesterarbeit.}
  \else
  \ifx\@worktype\@worktypeIP
    \def\@worktypetext{INGENIEURPRAXIS}
    \typeout{-- Type: Industriepraxis.}
  \else
  \ifx\@worktype\@worktypeFP
    \def\@worktypetext{FORSCHUNGSPRAXIS}
    \typeout{-- Type: Forschungspraxis.}
  \else
    \def\@worktypetext{ABSCHLUSSARBEIT}
    \typeout{-- Type: not specified.}
  \fi
  \fi
  \fi
  \fi
  \fi
  \fi
  \fi
  \fi
\else % english
  \typeout{-- language set to english.}
  \ifx\@worktype\@worktypeDA
    \def\@worktypetext{DIPLOMA THESIS}
    \typeout{-- Type: Diplomarbeit.}
  \else
  \ifx\@worktype\@worktypeBA
    \def\@worktypetext{BACHELOR'S THESIS}
    \typeout{-- Type: Bachelorarbeit.}
  \else
  \ifx\@worktype\@worktypeMA
    \def\@worktypetext{MASTER'S THESIS}
    \typeout{-- Type: Masterarbeit.}
  \else
  \ifx\@worktype\@worktypeHS
    \def\@worktypetext{ADVANCED SEMINAR}
    \typeout{-- Type: Advanced Seminar.}
  \else
  \ifx\@worktype\@worktypePP
    \def\@worktypetext{PRACTICAL COURSE}
    \typeout{-- Type: Projektpraktikum.} 
  \else
  \ifx\@worktype\@worktypeSeA
    \def\@worktypetext{SEMESTER THESIS}
    \typeout{-- Type: Semesterarbeit.}
  \else
  \ifx\@worktype\@worktypeIP
    \def\@worktypetext{INGENIEURPRAXIS}
    \typeout{-- Type: Industriepraxis.}
  \else
  \ifx\@worktype\@worktypeFP
    \def\@worktypetext{FORSCHUNGSPRAXIS}
    \typeout{-- Type: Forschungspraxis.}
  \else
    \def\@worktypetext{THESIS}
    \typeout{-- Type: not specified.}
    \fi
    \fi
    \fi
    \fi
    \fi
    \fi
    \fi
    \fi
    \fi
%___________________________________________

% 
%___________the student data variables______________
\def\@student{N.N.}
\newcommand{\student}[1]{\def\@student{\mbox{}#1}}
\def\@vartitle{}
\newcommand{\studtitle}[1]{\def\@vartitle{#1}}
\def\@studenttwo{\mbox{}}
\newcommand{\studenttwo}[1]{\def\@studenttwo{\mbox{}#1}}
\def\@vartitletwo{}
\newcommand{\studtitletwo}[1]{\def\@vartitletwo{#1}}
\def\@studentthree{\mbox{}}
\newcommand{\studentthree}[1]{\def\@studentthree{\mbox{}#1}}
\def\@vartitlethree{\mbox{}}
\newcommand{\studtitlethree}[1]{\def\@vartitlethree{\mbox{}#1}}
\def\@studentfour{\mbox{}}
\newcommand{\studentfour}[1]{\def\@studentfour{\mbox{}#1}}
\def\@vartitlefour{\mbox{}}
\newcommand{\studtitlefour}[1]{\def\@vartitlefour{\mbox{}#1}}

\def\@varyearone{DD. MM. YYYY}
\newcommand{\yearofbirth}[1]{\def\@varyearone{#1}}
\def\@varstreetone{Bakerstreet 221B}
\newcommand{\street}[1]{\def\@varstreetone{#1}}
\def\@varcity{8xxxx München}
\newcommand{\city}[1]{\def\@varcity{#1}}
\def\@vartele{089 - 1234567}
\newcommand{\phone}[1]{\def\@vartele{#1}}
% todo: additional informations for multiple students
%_____________________________________________________
\ifx\@institute\@instituteLSR
\def\@varprofessor{M. Buss}
\else
\ifx\@institute\@instituteITR
\def\@varprofessor{S. Hirche}
\else
\ifx\@institute\@instituteICS
\def\@varprofessor{G. Cheng}
\fi
\fi
\fi
\newcommand{\professor}[1]{\def\@varprofessor{\mbox{}#1}}
\def\@varprofessortype{Univ.-Professor}
\newcommand{\professortype}[1]{\def\@varprofessortype{\mbox{}#1}}

\def\@vartitletranslation{}
\newcommand{\titletranslation}[1]{\def\@vartitletranslation{#1}}


\def\@varsupervisor{Dipl.-Ing. N.N.}
\newcommand{\supervisor}[1]{\def\@varsupervisor{#1}}

\def\@varstart{xx.xx.xxxx}
\newcommand{\start}[1]{\def\@varstart{#1}}

\def\@varprog{xx.xx.xxxx}
\newcommand{\progrep}[1]{\def\@varprog{#1}}

\def\@varfinal{xx.xx.xxxx}
\newcommand{\finalrep}[1]{\def\@varfinal{#1}}






%___________________footer______________________

%%%%%%%%%%%%%%%%%%%%% F O O T E R %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\if@langGer
  \ifx\@worktype\@worktypeHS
  \newcommand{\footer}{
    \vspace*{1ex}
    \begin{tabular}{ll}
      Betreuer/-in: & \@varsupervisor\\
    \end{tabular}
  }
  \else
  \ifx\@worktype\@worktypePP
  \newcommand{\footer}{
    \vspace*{1ex}
    \begin{tabular}{ll}
      Betreuer/-in: & \@varsupervisor\\
    \end{tabular}
  }
  \else
  \ifx\@worktype\@worktypeIP
  \newcommand{\footer}{
    \vspace*{1ex}
    \begin{tabular}{ll}
      Betreuer/-in: & \@varsupervisor\\
      Beginn: & \@varstart\\
      Abgabe: & \@varfinal
    \end{tabular}
  }
  \else
  \ifx\@worktype\@worktypeFP
  \newcommand{\footer}{
    \vspace*{1ex}
    \begin{tabular}{ll}
      Betreuer/-in: & \@varsupervisor\\
      Beginn: & \@varstart\\
      Abgabe: & \@varfinal
    \end{tabular}
  }
  \else
  \newcommand{\footer}{
    \vspace*{1ex}
    \if@typeFinal
    \begin{tabular}{ll}
      Betreuer/-in: & \@varsupervisor\\
      Beginn: & \@varstart\\
      Zwischenbericht: & \@varprog\\
      Abgabe: & \@varfinal
    \end{tabular}
    \else
    \begin{tabular}{ll}
      Betreuer/-in: & \@varsupervisor\\
      Beginn: & \@varstart\\
      Zwischenbericht: & \@varprog
    \end{tabular}
    \fi
  }
  \fi
  \fi
  \fi
  \fi
\else
   \ifx\@worktype\@worktypeHS
   \newcommand{\footer}{
     \vspace*{1ex}
     \begin{tabular}{ll}
       Supervisor: & \@varsupervisor\\
       Start: & \@varstart\\
       Delivery: & \@varfinal
      \end{tabular}
   }
   \else
   \ifx\@worktype\@worktypePP
   \newcommand{\footer}{
     \vspace*{1ex}
     \begin{tabular}{ll}
       Supervisor: & \@varsupervisor\\
       Start: & \@varstart\\
       Delivery: & \@varfinal
      \end{tabular}
   }
   \else
   \ifx\@worktype\@worktypeIP
   \newcommand{\footer}{
     \vspace*{1ex}
     \begin{tabular}{ll}
       Supervisor: & \@varsupervisor\\
       Start: & \@varstart\\
       Delivery: & \@varfinal
     \end{tabular}
   }
   \else
   \ifx\@worktype\@worktypeFP
   \newcommand{\footer}{
     \vspace*{1ex}
     \begin{tabular}{ll}
       Supervisor: & \@varsupervisor\\
       Start: & \@varstart\\
       Delivery: & \@varfinal
     \end{tabular}
   }
   \else
   \newcommand{\footer}{
     \vspace*{1ex}
     \if@typeFinal 
     \begin{tabular}{ll}
       Supervisor: & \@varsupervisor\\
       Start: & \@varstart\\
       Intermediate Report: & \@varprog\\
       Delivery: & \@varfinal
     \end{tabular}
     \else
     \begin{tabular}{ll}
       Supervisor: & \@varsupervisor\\
       Start: & \@varstart\\
       Intermediate Report: & \@varprog
     \end{tabular}  
     \fi
   }
\fi
\fi
\fi
\fi
\fi
\pagestyle{empty}
%%%%%%%%%%%%%%%%%%%%% M A K E T I T L E %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\renewcommand{\maketitle}{
	\enlargethispage{4.5cm} %Damit das Titelbild weit genug unten ist!
	\begin{center}
	\phantom{u}
	\vspace{0.5cm}
	\Huge{\sc \@title}\\
\ifx\@vartitletranslation\empty
	{}
\else
	\large{\sc (\@vartitletranslation) \\}
\fi
	\vspace{1.5cm}
\if@typeFinal
	\if@langGer
		\large{eingereichte\\ \@worktypetext \\}
	\else
		\large{submitted\\ \@worktypetext \\}	
	\fi
\else
	\if@langGer
		\large{Zwischenbericht zur\\ \@worktypetext \\}
	\else
		\large{Intermediate report on the\\ \@worktypetext \\}	
	\fi
\fi

	\vspace{0.4cm}
	\@vartitle~\@student\\
	\vspace{0.5cm}
\if@langGer
	geb. am \@varyearone\\
	wohnhaft in:\\
\else
	born \@varyearone\\
	resident in:\\
\fi	
	\@varstreetone \\
	\@varcity \\
	Tel.: \@vartele \\	
	\vspace{1.5cm}
\if@langGer
  \ifx\@institute\@instituteLSR
	Lehrstuhl f\"ur\\
	STEUERUNGS- und REGELUNGSTECHNIK\\
	Technische Universit\"at M\"unchen\\
  \else
  \ifx\@institute\@instituteITR
  Lehrstuhl f\"ur\\
  INFORMATIONSTECHNISCHE REGELUNG \\
  Technische Universit\"at M\"unchen\\
  \else
  \ifx\@institute\@instituteICS
  Lehrstuhl f\"ur\\
  KONGNITIVE SYSTEME \\
  Technische Universit\"at M\"unchen\\
  \fi
  \fi
  \fi
\else
\ifx\@institute\@instituteLSR
  Chair of \\
  AUTOMATIC CONTROL ENGINEERING\\
  Technical University of Munich\\
  \else
  \ifx\@institute\@instituteITR
  Chair of \\
  INFORMATION-ORIENTED CONTROL\\
  Technical University of Munich\\
  \else
  \ifx\@institute\@instituteICS
  Institute for \\
  CONGNITIVE SYSTEMS \\
  Technical University of Munich\\
  \fi
  \fi
  \fi
\fi
	\vspace{0.3cm}
\ifx\@institute\@instituteITR
	Univ.-Prof.~Dr.-Ing. Sandra Hirche
  \else	
  \ifx\@institute\@instituteLSR
  	Univ.-Prof.~Dr.-Ing./Univ.~Tokio Martin Buss
  	Univ.-Prof.~Dr.-Ing.~Dirk Wollherr
  \else	
  \ifx\@institute\@instituteICS
	Prof. Dr. Gordon Cheng
  \fi
  \fi
\fi	
	\end{center} 
	\vspace{2.5cm}
	\footer	
}


%____________________________________________________________