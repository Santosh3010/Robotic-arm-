
// BlockMovingRobotDlg.h : header file
//

#pragma once
using namespace std;
#include <list>
#include <stack>

#define HAND_PICKUP     1
#define HAND_PICKDOWN   2
#define HAND_STACK      3
#define HAND_UNSTACK    4
#define HAND_MOVE       5
#define HAND_NOOP       6

struct BLOCK
{
	int s_x, s_y;
	int s_level;
	int s_block_id;
	int s_table_number;
	CString s_box_name;
	void moveLeft()
	{
		s_x -= 5;
	}
	void moveRight()
	{
		s_x += 5;
	}
	void moveTop()
	{
		s_y -= 5;
	}
	void moveBottom()
	{
		s_y += 5;
	}
};

struct ROBOT_HAND
{
	POINT s_pt[12];	// pt_0 : top, pt_7 : block
	int s_length;
	CString strName;
	int s_state;
	int s_block_id;
	void moveLeft()
	{
		for (int i = 0; i < 12; i++) {
			s_pt[i].x -= 5;
		}
	}	
	void moveRight()
	{
		for (int i = 0; i < 12; i++) {
			s_pt[i].x += 5;
		}
	}
	void moveTop()
	{
		for (int i = 0; i < 12; i++) {
			s_pt[i].y -= 5;
		}
	}
	void moveBottom()
	{
		for (int i = 0; i < 12; i++) {
			s_pt[i].y += 5;
		}
	}
};

// CBlockMovingRobotDlg dialog
class CBlockMovingRobotDlg : public CDialogEx
{
// Construction
public:
	CBlockMovingRobotDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLOCKMOVINGROBOT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_block_count;
	ROBOT_HAND m_handL, m_handR;
	CComboBox m_cmb_action;
	CComboBox m_cmb_block;
	CComboBox m_cmb_dst_table;
	CComboBox m_cmb_robot_hand;

	CDC* m_dcPan;
	CWnd* m_wndPan;
	RECT m_rtClear;
	CBrush m_brFill;

	afx_msg void OnBnClickedBtnReset();

	// block array
	BLOCK m_block_array[100];
	stack<int> m_stackTbLeft;
	stack<int> m_stackTbRight;

	void clearBackground();
	void drawCurrentState();
	void resetItems();
	CString m_str_action;
	afx_msg void OnCbnSelchangeCmbBlock();
	afx_msg void OnBnClickedBtnStart();
	void moveBlockToRightTable();
	void stack1BlockRightHand();
	void goAndStackLeftHand();
	void moveBlockToLeftTable();
	void noopRightHand();
	void moveLStackBlockToRightTable();
	void noopLeftHand();
	void goAndStackRightHand();
	void moveLStackBlockToLeftTable();
};
