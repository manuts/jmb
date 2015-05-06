\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{dks-report}

\newcommand{\chairname}{Digitale Kommunikationssysteme}
\newcommand{\uniname}{Ruhr-Universit{\"a}t Bochum}
\ProcessOptions\relax
\LoadClass[a4paper]{article}
\RequirePackage{graphicx}
\RequirePackage{fancyhdr}
\RequirePackage{xcolor}

%define site geometry
\RequirePackage[left=2cm,right=2cm,top=0.2cm,bottom=4.4cm, includeheadfoot]{geometry}

%define colors
\definecolor{logoColor}{RGB}{146,192,25}
\definecolor{chairColor}{RGB}{0,50,100}

%clear header
\fancyhf{} 

%header space
\fancyheadoffset[L, R]{1.4cm} % make header wider then textwidth
\addtolength{\headheight}{2.5cm} % make more space for the header

%header place
\pagestyle{fancyplain} % use fancy for all pages except chapter start

%header content
\lhead{
	\includegraphics[height=2.4cm]{figures/dks-logo.png}\hspace{0.5cm}\textcolor{chairColor}{\textbf{\Large \chairname}}
	} % left 
\chead{} % center
\rhead{\includegraphics[height=2.4cm]{figures/logo_rub.png}} % right 

%line between header and text
\renewcommand\headrule{
	\color{logoColor}
	\vspace{0pt}
	\hrule height 6pt width\headwidth
}

\fancyfootoffset[L, R]{1.3cm} % make header wider then textwidth

\renewcommand\footrule{
	\color{logoColor}
	\vspace{0pt}
	\hrule height 6pt width\headwidth
}

%left footer
\lfoot{\textcolor{chairColor}{\normalsize \chairname \\ \small \uniname\\
Universit{\"a}tsstra{\ss}e  150\\ D-44780 Bochum, Deutschland\\}}

%right footer
\rfoot{ %
	  \begin{tabular}[t]{ll}
		\textcolor{chairColor}{Contact:} & \textcolor{chairColor}{\@author} \\ \textcolor{chairColor}{Mail:} & \textcolor{chairColor}{\mail} \\
		\textcolor{chairColor}{Room:} & \textcolor{chairColor}{\room} \\
		\textcolor{chairColor}{Phone:} & \textcolor{chairColor}{+49234 32-\tele} \\
	  \end{tabular}}

%center footer	  
\cfoot{
	\textcolor{chairColor}{\footnotesize \@date\\
	\scriptsize www.dks.ruhr-uni-bochum.de}
	}
