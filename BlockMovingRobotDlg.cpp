
// BlockMovingRobotDlg.cpp : implementation file
//


#include "pch.h"
#include "framework.h"
#include "BlockMovingRobot.h"
#include "BlockMovingRobotDlg.h"
#include "afxdialogex.h"
#include <stack>

#define TBL_X1 150
#define TBL_X2 350
#define TBL_Y 500
#define TBL_LENGTH 350
#define BLOCK_WH 30

#define HAND_MIN_LEN 60
#define HAND_MAX_LEN 100
#define HAND_WIDTH 10
#define HAND_WIDTH_H HAND_WIDTH/2

#define HAND_L0X TBL_X1 + (BLOCK_WH-HAND_WIDTH)/2
#define HAND_L0Y 50
#define HAND_R0X TBL_X2 + (BLOCK_WH-HAND_WIDTH)/2
#define HAND_R0Y 50

#define DELAY_TIME 30

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBlockMovingRobotDlg dialog



CBlockMovingRobotDlg::CBlockMovingRobotDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BLOCKMOVINGROBOT_DIALOG, pParent)
	, m_block_count(0)
	, m_str_action(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBlockMovingRobotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_TBL_COUNT, m_block_count);
	DDV_MinMaxInt(pDX, m_block_count, 1, 10);
	DDX_Control(pDX, IDC_CMB_ACTION, m_cmb_action);
	DDX_Control(pDX, IDC_CMB_BLOCK, m_cmb_block);
	DDX_Control(pDX, IDC_CMB_DST_TABLE, m_cmb_dst_table);
	DDX_Control(pDX, IDC_CMB_ROBOT_HAND, m_cmb_robot_hand);
	DDX_CBString(pDX, IDC_CMB_ACTION, m_str_action);
}

BEGIN_MESSAGE_MAP(CBlockMovingRobotDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RESET, &CBlockMovingRobotDlg::OnBnClickedBtnReset)
	ON_CBN_SELCHANGE(IDC_CMB_BLOCK, &CBlockMovingRobotDlg::OnCbnSelchangeCmbBlock)
	ON_BN_CLICKED(IDC_BTN_START, &CBlockMovingRobotDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CBlockMovingRobotDlg message handlers

BOOL CBlockMovingRobotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_cmb_action.AddString(L"Move");
	m_cmb_action.AddString(L"Stack");
	m_cmb_action.SetCurSel(0);

	m_cmb_dst_table.AddString(L"Left Table");
	m_cmb_dst_table.AddString(L"Right Table");
	m_cmb_dst_table.SetCurSel(0);

	// TODO: Add extra initialization here

	m_wndPan = GetDlgItem(IDC_STATIC_GRAPH);
	m_dcPan = m_wndPan->GetDC();
	m_dcPan->SetBkMode(TRANSPARENT);
	m_wndPan->GetClientRect(&m_rtClear);
	m_rtClear.left += 1;
	m_rtClear.right -= 1;
	m_rtClear.top += 1;
	m_rtClear.bottom -= 1;

	m_brFill.CreateSolidBrush(RGB(255, 255, 255));
	m_block_count = 7;
	UpdateData(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlockMovingRobotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBlockMovingRobotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;


		CWnd* drawWnd = GetDlgItem(IDC_STATIC_GRAPH);
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{		
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBlockMovingRobotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBlockMovingRobotDlg::OnBnClickedBtnReset()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
	if (m_block_count < 1)	return;

	clearBackground();
	resetItems();
	drawCurrentState();
}

void CBlockMovingRobotDlg::clearBackground()
{
	// TODO: Add your implementation code here.
	m_dcPan->FillRect(&m_rtClear, &m_brFill);
}

void CBlockMovingRobotDlg::drawCurrentState()
{
	// TODO: Add your implementation code here.

	/* Draw Table Lines */

	CPen cpTbl, cpBlock;
	cpTbl.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	cpBlock.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	m_dcPan->SelectObject(&cpTbl);
	m_dcPan->MoveTo(TBL_X1 - 50, TBL_Y);
	m_dcPan->LineTo(TBL_X1 - 50 + TBL_LENGTH, TBL_Y);

	/* Draw Table Names */

	RECT rt;
	rt.left = TBL_X1 + 20;
	rt.right = TBL_X1 + 80;
	rt.top = TBL_Y;
	rt.bottom = TBL_Y + 50;
	m_dcPan->DrawText(L"L1", &rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	rt.left = TBL_X2 + 20;
	rt.right = TBL_X2 + 80;
	rt.top = TBL_Y;
	rt.bottom = TBL_Y + 50;
	m_dcPan->DrawText(L"L2", &rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	/* Draw Blocks */

	CString temp;
	m_dcPan->SelectObject(&cpBlock);
	for (int i = 0; i < m_block_count; i++)
	{
		rt.left = m_block_array[i].s_x;
		rt.right = m_block_array[i].s_x + BLOCK_WH;
		rt.top = m_block_array[i].s_y;
		rt.bottom = m_block_array[i].s_y+BLOCK_WH;
		m_dcPan->Rectangle(&rt);
		m_dcPan->DrawText(m_block_array[i].s_box_name, &rt, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}

	/* Draw Robots hand */

	CBrush brHand;
	RECT rtHand;
	rtHand.left = m_handL.s_pt[0].x;
	rtHand.right = m_handL.s_pt[2].x;
	rtHand.top = m_handL.s_pt[0].y;
	rtHand.bottom = m_handL.s_pt[2].y;
	brHand.CreateSolidBrush(RGB(200, 200, 200));
	m_dcPan->SelectObject(&brHand);
	m_dcPan->Polygon(m_handL.s_pt, 12);
	m_dcPan->DrawText(L"L", &rtHand, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	rtHand.left = m_handR.s_pt[0].x;
	rtHand.right = m_handR.s_pt[2].x;
	rtHand.top = m_handR.s_pt[0].y;
	rtHand.bottom = m_handR.s_pt[2].y;
	m_dcPan->Polygon(m_handR.s_pt, 12);
	m_dcPan->DrawText(L"R", &rtHand, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CBlockMovingRobotDlg::resetItems()
{
	// TODO: Add your implementation code here.

	m_cmb_block.ResetContent();
	m_cmb_robot_hand.EnableWindow(false);
	m_stackTbLeft.empty();
	m_stackTbRight.empty();

	CString temp;
	for (int i = 0; i < m_block_count; i++)
	{
		temp.Format(L"%c", 'a' + i);
		m_block_array[i].s_level = i + 1;
		m_block_array[i].s_block_id = i + 1;
		m_block_array[i].s_table_number = 2;
		m_block_array[i].s_box_name = temp;
		m_block_array[i].s_x = TBL_X2 + HAND_WIDTH;
		m_block_array[i].s_y = TBL_Y - BLOCK_WH * (i + 1);
		m_cmb_block.AddString(temp);
		//m_stackTbLeft.push(i + 1);
		m_stackTbRight.push(i + 1);
	}
	m_cmb_block.SetCurSel(0);

	/* Set Hand L  Position */

	m_handL.s_pt[0].x = HAND_L0X;
	m_handL.s_pt[0].y = HAND_L0Y;
	m_handL.s_pt[1].x = m_handL.s_pt[0].x + HAND_WIDTH;
	m_handL.s_pt[1].y = m_handL.s_pt[0].y;
	m_handL.s_pt[2].x = m_handL.s_pt[0].x + HAND_WIDTH;
	m_handL.s_pt[2].y = m_handL.s_pt[0].y + HAND_MIN_LEN;
	m_handL.s_pt[11].x = m_handL.s_pt[0].x;
	m_handL.s_pt[11].y = m_handL.s_pt[0].y + HAND_MIN_LEN;

	m_handL.s_pt[3].x = m_handL.s_pt[0].x + HAND_WIDTH + HAND_WIDTH_H + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[3].y = m_handL.s_pt[0].y + HAND_MIN_LEN;
	m_handL.s_pt[10].x = m_handL.s_pt[0].x - HAND_WIDTH_H - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[10].y = m_handL.s_pt[0].y + HAND_MIN_LEN;

	m_handL.s_pt[4].x = m_handL.s_pt[0].x + HAND_WIDTH + HAND_WIDTH_H + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[4].y = m_handL.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 2;
	m_handL.s_pt[9].x = m_handL.s_pt[0].x - HAND_WIDTH_H - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[9].y = m_handL.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 2;

	m_handL.s_pt[5].x = m_handL.s_pt[0].x + HAND_WIDTH + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[5].y = m_handL.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 3;
	m_handL.s_pt[8].x = m_handL.s_pt[0].x - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[8].y = m_handL.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 3;

	m_handL.s_pt[6].x = m_handL.s_pt[0].x + HAND_WIDTH + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[6].y = m_handL.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H;
	m_handL.s_pt[7].x = m_handL.s_pt[0].x - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handL.s_pt[7].y = m_handL.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H;


	m_handR.s_pt[0].x = HAND_R0X + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[0].y = HAND_R0Y;
	m_handR.s_pt[1].x = m_handR.s_pt[0].x + HAND_WIDTH;
	m_handR.s_pt[1].y = m_handR.s_pt[0].y;
	m_handR.s_pt[2].x = m_handR.s_pt[0].x + HAND_WIDTH;
	m_handR.s_pt[2].y = m_handR.s_pt[0].y + HAND_MIN_LEN;
	m_handR.s_pt[11].x = m_handR.s_pt[0].x;
	m_handR.s_pt[11].y = m_handR.s_pt[0].y + HAND_MIN_LEN;

	m_handR.s_pt[3].x = m_handR.s_pt[0].x + HAND_WIDTH + HAND_WIDTH_H + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[3].y = m_handR.s_pt[0].y + HAND_MIN_LEN;
	m_handR.s_pt[10].x = m_handR.s_pt[0].x - HAND_WIDTH_H - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[10].y = m_handR.s_pt[0].y + HAND_MIN_LEN;

	m_handR.s_pt[4].x = m_handR.s_pt[0].x + HAND_WIDTH + HAND_WIDTH_H + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[4].y = m_handR.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 2;
	m_handR.s_pt[9].x = m_handR.s_pt[0].x - HAND_WIDTH_H - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[9].y = m_handR.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 2;

	m_handR.s_pt[5].x = m_handR.s_pt[0].x + HAND_WIDTH + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[5].y = m_handR.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 3;
	m_handR.s_pt[8].x = m_handR.s_pt[0].x - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[8].y = m_handR.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H * 3;

	m_handR.s_pt[6].x = m_handR.s_pt[0].x + HAND_WIDTH + (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[6].y = m_handR.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H;
	m_handR.s_pt[7].x = m_handR.s_pt[0].x - (BLOCK_WH - HAND_WIDTH) / 2;
	m_handR.s_pt[7].y = m_handR.s_pt[0].y + HAND_MIN_LEN + HAND_WIDTH_H;

	m_handL.strName = L"L";
	m_handL.s_length = 80;
	m_handL.s_state = HAND_NOOP;
}

void CBlockMovingRobotDlg::OnCbnSelchangeCmbBlock()
{
	// TODO: Add your control notification handler code here
	CString str, tmp;
	GetDlgItemText(IDC_CMB_BLOCK, str);
	int currentTableNum;
	for (int i=0; i<m_block_count; i++)
	{
		tmp.Format(L"%c", 'a' + i);
		if (tmp == str)
		{
			currentTableNum = m_block_array[i].s_table_number;
			break;
		}
	}
	//m_cmb_dst_table.SetCurSel(2 - currentTableNum);
	m_cmb_dst_table.SetCurSel(2 - currentTableNum);
}

void CBlockMovingRobotDlg::OnBnClickedBtnStart()
{
	// TODO: Add your control notification handler code here

	/* Block Move Event */

	int prepareCount = 0;

	if (m_cmb_action.GetCurSel() == 0)
	{
		/* Get List of pre moved blocks */
		int selectBlockID, selectBlockTableID, selectBlockLevel;
		for (int i = 0; i < m_block_count; i++)
		{
			CString str, tmp;
			GetDlgItemText(IDC_CMB_BLOCK, str);
			char tmpChar = str.GetAt(0);
			if (m_block_array[i].s_block_id == (tmpChar - 'a' + 1))
			{
				selectBlockTableID = m_block_array[i].s_table_number;
				selectBlockID = i + 1;
				break;
			}
		}
		/* Move all lists of block to Other Side */

		if (selectBlockTableID == 1)		//  Left  <<<<<<<<<<<---------     Right
		{
			if (m_stackTbLeft.top() == selectBlockID)
			{
				moveBlockToRightTable();
				noopLeftHand();
				return;
			}
			while (m_stackTbLeft.top() != selectBlockID )
			{
				moveBlockToRightTable();
				prepareCount++;
			}
			goAndStackLeftHand();

			while (prepareCount != 0)
			{
				moveBlockToLeftTable();
				prepareCount--;
			}
			noopRightHand();
			moveLStackBlockToRightTable();
			noopLeftHand();
		}
		if (selectBlockTableID == 2)		//  Left  --------->>>>>>>>>>>>     Right
		{
			if (m_stackTbRight.top() == selectBlockID)
			{
				moveBlockToLeftTable();
				noopRightHand();
				return;
			}
			while (m_stackTbRight.top() != selectBlockID)
			{
				moveBlockToLeftTable();
				prepareCount++;
			}
			goAndStackRightHand();

			while (prepareCount != 0)
			{
				moveBlockToRightTable();
				prepareCount--;
			}
			noopLeftHand();
			moveLStackBlockToLeftTable();
			noopRightHand();

		}
		/* Stack Target Block to other Hand */


		/* Move back lists of blocks to Right */


		/* Unstack to target table */
		/* Return robot hand to origin location */
	}
}


void CBlockMovingRobotDlg::moveBlockToLeftTable()
{
	// TODO: Add your implementation code here.
		// TODO: Add your implementation code here.
	if (m_stackTbRight.size() == 0) return;

	int distX, distY;
	int tempX, tempY;
	int blockID = m_stackTbRight.top();

	if (m_handR.s_pt[7].y > m_block_array[blockID - 1].s_y - BLOCK_WH)
	{
		distY = m_handR.s_pt[7].y - m_block_array[blockID - 1].s_y + BLOCK_WH;
		/* Attach to Current Block */
		if (distY > 0)
		{
			while (distY != 0 || distY < 0)
			{
				m_handR.moveTop();
				distY -= 5;
				Sleep(DELAY_TIME);
				clearBackground();
				drawCurrentState();
			}
		}
	}

	int blockX = m_block_array[blockID - 1].s_x;
	int blockY = m_block_array[blockID - 1].s_y;
	distX = m_handR.s_pt[7].x - blockX;
	distY = m_handR.s_pt[7].y - blockY;

	/* Attach to Current Block */
	if (distX < 0)
	{
		while (distX != 0)
		{
			m_handR.moveRight();
			distX += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distX > 0)
	{
		while (distX != 0)
		{
			m_handR.moveLeft();
			distX -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY > 0)
	{
		while (distY != 0 || distY < 0)
		{
			m_handR.moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handR.moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}

	/* Drag Block about a Block Height */

	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handR.moveTop();
		m_block_array[blockID - 1].moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	distY = m_handR.s_pt[7].y - (TBL_Y - (m_stackTbLeft.size() + 2) * BLOCK_WH);
	if (distY > 0)
	{
		while (distY != 0)
		{
			m_handR.moveTop();
			m_block_array[blockID - 1].moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	/* Move to Left to L2  */
	distX = -(m_handR.s_pt[7].x - HAND_L0X - (BLOCK_WH - HAND_WIDTH) / 2);
	if (distX < 0)
	{
		while (distX != 0)
		{
			m_handR.moveLeft();
			m_block_array[blockID - 1].moveLeft();
			distX += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distX > 0)
	{
		while (distX != 0)
		{
			m_handR.moveRight();
			m_block_array[blockID - 1].moveRight();
			distX -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}

	/* Move Down to L2  */
	distY = TBL_Y - m_handR.s_pt[7].y - (m_stackTbLeft.size() + 1) * BLOCK_WH;
	while (distY != 0)
	{
		m_handR.moveBottom();
		m_block_array[blockID - 1].moveBottom();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}


	m_block_array[blockID - 1].s_table_number = 1;
	m_stackTbLeft.push(m_stackTbRight.top());
	m_stackTbRight.pop();

	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handR.moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}
}

void CBlockMovingRobotDlg::moveBlockToRightTable()
{
	// TODO: Add your implementation code here.
	if (m_stackTbLeft.size() == 0) return;

	int distX, distY;
	int tempX, tempY;
	int blockID = m_stackTbLeft.top();

	if (m_handL.s_pt[7].y > m_block_array[blockID - 1].s_y - BLOCK_WH)
	{
		distY = m_handL.s_pt[7].y - m_block_array[blockID - 1].s_y + BLOCK_WH;
		/* Attach to Current Block */
		if (distY > 0)
		{
			while (distY != 0 || distY < 0)
			{
				m_handL.moveTop();
				distY -= 5;
				Sleep(DELAY_TIME);
				clearBackground();
				drawCurrentState();
			}
		}
	}

	int blockX = m_block_array[blockID - 1].s_x;
	int blockY = m_block_array[blockID - 1].s_y;
	distX = m_handL.s_pt[7].x - blockX;
	distY = m_handL.s_pt[7].y - blockY;

	/* Attach to Current Block */
	if (distX < 0)
	{
		while (distX != 0)
		{
			m_handL.moveRight();
			distX += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distX > 0)
	{
		while (distX != 0)
		{
			m_handL.moveLeft();
			distX -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY > 0)
	{
		while (distY != 0 || distY < 0)
		{
			m_handL.moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handL.moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}

	/* Drag Block about a Block Height */

	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handL.moveTop();
		m_block_array[blockID - 1].moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	distY = m_handL.s_pt[7].y - (TBL_Y - (m_stackTbRight.size() + 2) * BLOCK_WH);
	if (distY > 0)
	{
		while (distY != 0)
		{
			m_handL.moveTop();
			m_block_array[blockID - 1].moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	/* Move to Right to L2  */
	distX = HAND_R0X - m_handL.s_pt[7].x;
	while (distX != 0)
	{
		m_handL.moveRight();
		m_block_array[blockID - 1].moveRight();
		distX -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	/* Move Down to L2  */
	distY = TBL_Y - m_handL.s_pt[7].y - (m_stackTbRight.size() + 1) * BLOCK_WH;
	while (distY != 0)
	{
		m_handL.moveBottom();
		m_block_array[blockID - 1].moveBottom();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	m_block_array[blockID - 1].s_table_number = 2;
	m_stackTbRight.push(m_stackTbLeft.top());
	m_stackTbLeft.pop();

	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handL.moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}
}


void CBlockMovingRobotDlg::stack1BlockRightHand()
{
	// TODO: Add your implementation code here.

}


void CBlockMovingRobotDlg::goAndStackLeftHand()
{
	// TODO: Add your implementation code here.
	if (m_stackTbLeft.size() == 0) return;

	int distX, distY;
	int tempX, tempY;
	int blockID = m_stackTbLeft.top();

	if (m_handL.s_pt[7].y > m_block_array[blockID - 1].s_y - BLOCK_WH)
	{
		distY = m_handL.s_pt[7].y - m_block_array[blockID - 1].s_y + BLOCK_WH;
		/* Attach to Current Block */
		if (distY > 0)
		{
			while (distY != 0 || distY < 0)
			{
				m_handL.moveTop();
				distY -= 5;
				Sleep(DELAY_TIME);
				clearBackground();
				drawCurrentState();
			}
		}
	}

	int blockX = m_block_array[blockID - 1].s_x;
	int blockY = m_block_array[blockID - 1].s_y;
	distX = m_handL.s_pt[7].x - blockX;
	distY = m_handL.s_pt[7].y - blockY;

	/* Attach to Current Block */
	if (distX < 0)
	{
		while (distX != 0)
		{
			m_handL.moveRight();
			distX += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distX > 0)
	{
		while (distX != 0)
		{
			m_handL.moveLeft();
			distX -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY > 0)
	{
		while (distY != 0 || distY < 0)
		{
			m_handL.moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handL.moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}

	/* Drag Up */
	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handL.moveTop();
		m_block_array[blockID - 1].moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}
	/*  */
	distY = m_handL.s_pt[7].y - (HAND_L0Y + HAND_MIN_LEN + HAND_WIDTH_H);
	if (distY > 0)
	{
		while (distY != 0)
		{
			m_handL.moveTop();
			m_block_array[blockID - 1].moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	m_handL.s_state = HAND_STACK;
	m_handL.s_block_id = blockID;
	m_stackTbLeft.pop();
}

void CBlockMovingRobotDlg::goAndStackRightHand()
{
	// TODO: Add your implementation code here.
	if (m_stackTbRight.size() == 0) return;

	int distX, distY;
	int tempX, tempY;
	int blockID = m_stackTbRight.top();

	if (m_handR.s_pt[7].y > m_block_array[blockID - 1].s_y - BLOCK_WH)
	{
		distY = m_handR.s_pt[7].y - m_block_array[blockID - 1].s_y + BLOCK_WH;
		/* Attach to Current Block */
		if (distY > 0)
		{
			while (distY != 0 || distY < 0)
			{
				m_handR.moveTop();
				distY -= 5;
				Sleep(DELAY_TIME);
				clearBackground();
				drawCurrentState();
			}
		}
	}

	int blockX = m_block_array[blockID - 1].s_x;
	int blockY = m_block_array[blockID - 1].s_y;
	distX = m_handR.s_pt[7].x - blockX;
	distY = m_handR.s_pt[7].y - blockY;

	/* Attach to Current Block */
	if (distX < 0)
	{
		while (distX != 0)
		{
			m_handR.moveRight();
			distX += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distX > 0)
	{
		while (distX != 0)
		{
			m_handR.moveLeft();
			distX -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY > 0)
	{
		while (distY != 0 || distY < 0)
		{
			m_handR.moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handR.moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}

	/* Drag Up */
	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handR.moveTop();
		m_block_array[blockID - 1].moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}
	/*  */
	distY = m_handR.s_pt[7].y - (HAND_R0Y + HAND_MIN_LEN + HAND_WIDTH_H);
	if (distY > 0)
	{
		while (distY != 0)
		{
			m_handR.moveTop();
			m_block_array[blockID - 1].moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	m_handR.s_state = HAND_STACK;
	m_handR.s_block_id = blockID;
	m_stackTbRight.pop();
}


void CBlockMovingRobotDlg::noopRightHand()
{
	// TODO: Add your implementation code here.
	int distX, distY;


	distY = (HAND_L0Y + HAND_MIN_LEN * 2 + BLOCK_WH + HAND_WIDTH) - m_handR.s_pt[7].y;
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handR.moveTop();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}


	distX = m_handR.s_pt[7].x - (HAND_R0X + (BLOCK_WH - HAND_WIDTH) / 2);
	distY = m_handR.s_pt[7].y - (HAND_R0Y + HAND_MIN_LEN + HAND_WIDTH_H);

	/* Attach to Current Block */
	if (distX < 0)
	{
		while (distX != 0)
		{
			m_handR.moveRight();
			distX += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distX > 0)
	{
		while (distX != 0)
		{
			m_handR.moveLeft();
			distX -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY > 0)
	{
		while (distY != 0 || distY < 0)
		{
			m_handR.moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handR.moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
}

void CBlockMovingRobotDlg::noopLeftHand()
{
	// TODO: Add your implementation code here.
	int distX, distY;

	distY = (HAND_L0Y + HAND_MIN_LEN*2 + BLOCK_WH + HAND_WIDTH) - m_handL.s_pt[7].y;
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handL.moveTop();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}

	distX = m_handL.s_pt[7].x - HAND_L0X;
	distY = m_handL.s_pt[7].y - (HAND_L0Y + HAND_MIN_LEN + HAND_WIDTH_H);

	/* Attach to Current Block */
	if (distX < 0)
	{
		while (distX != 0)
		{
			m_handL.moveRight();
			distX += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distX > 0)
	{
		while (distX != 0)
		{
			m_handL.moveLeft();
			distX -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY > 0)
	{
		while (distY != 0 || distY < 0)
		{
			m_handL.moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handL.moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
}


void CBlockMovingRobotDlg::moveLStackBlockToRightTable()
{
	// TODO: Add your implementation code here.
	int distX, distY;
	distY = -(HAND_MIN_LEN + HAND_WIDTH + BLOCK_WH);
	if (distY > 0)
	{
		while (distY != 0)
		{
			m_handL.moveTop();
			m_block_array[m_handL.s_block_id - 1].moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handL.moveBottom();
			m_block_array[m_handL.s_block_id - 1].moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	
	/* Move to Right to L2  */
	distX = HAND_R0X - m_handL.s_pt[7].x;
	while (distX != 0)
	{
		m_handL.moveRight();
		m_block_array[m_handL.s_block_id - 1].moveRight();
		distX -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	/* Move Down to L2  */
	distY = TBL_Y - m_handL.s_pt[7].y - (m_stackTbRight.size() + 1) * BLOCK_WH;
	while (distY != 0)
	{
		m_handL.moveBottom();
		m_block_array[m_handL.s_block_id - 1].moveBottom();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	m_block_array[m_handL.s_block_id - 1].s_table_number = 2;
	m_stackTbRight.push(m_handL.s_block_id);

	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handL.moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}
}

void CBlockMovingRobotDlg::moveLStackBlockToLeftTable()
{
	// TODO: Add your implementation code here.
	int distX, distY;
	distY = -(HAND_MIN_LEN + HAND_WIDTH + BLOCK_WH);
	if (distY > 0)
	{
		while (distY != 0)
		{
			m_handR.moveTop();
			m_block_array[m_handR.s_block_id - 1].moveTop();
			distY -= 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	if (distY < 0)
	{
		while (distY != 0)
		{
			m_handR.moveBottom();
			m_block_array[m_handR.s_block_id - 1].moveBottom();
			distY += 5;
			Sleep(DELAY_TIME);
			clearBackground();
			drawCurrentState();
		}
	}
	/* Move to Right to L2  */
	distX = TBL_X1 - m_handR.s_pt[7].x;
// 	if(m_stackTbLeft.size() > 0)
// 		distX = m_block_array[m_stackTbLeft.top()-1].s_x -  m_handR.s_pt[7].x;
	while (distX != 0)
	{
		m_handR.moveLeft();
		m_block_array[m_handR.s_block_id - 1].moveLeft();
		distX += 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	/* Move Down to L2  */
	distY = TBL_Y - m_handR.s_pt[7].y - (m_stackTbLeft.size() + 1) * BLOCK_WH;
	while (distY != 0)
	{
		m_handR.moveBottom();
		m_block_array[m_handR.s_block_id - 1].moveBottom();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}

	m_block_array[m_handR.s_block_id - 1].s_table_number = 1;
	m_stackTbLeft.push(m_handR.s_block_id);

	distY = BLOCK_WH;
	while (distY != 0)
	{
		m_handR.moveTop();
		distY -= 5;
		Sleep(DELAY_TIME);
		clearBackground();
		drawCurrentState();
	}
}
