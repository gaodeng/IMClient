#ifndef UILISTCOMMONDEFINE_HPP
#define UILISTCOMMONDEFINE_HPP

#include <math.h>

//struct FriendListItemInfo;//

#define  FRIEND_ONLINE		0x01
#define  FRIEND_OFFLINE		0x02
#define  FRIEND_LEAVE       0x04
#define  FRIEND_BUSY		0x08
#define  FRIEND_UNDISTURB   0x10
#define  FRIEND_HIDE        0x20



class FriendListItemInfo
{
public:
	FriendListItemInfo();
	~FriendListItemInfo();
	FriendListItemInfo(const FriendListItemInfo& obj);
	//����չ�����߲�չ��
	bool folder; 
	//Item�Ƿ�Ϊ��
	bool empty; 
	//Id
	tString id;
	//���ͼ��
	tString logo; 
	//�ǳ�
	tString nick_name;
	//���Ҳ�����
	tString description;
	//��ID�� Ŀǰû�ô�
	tString  groupId;
	//������
	tString goupName; 
	//�ƶ��绰
	tString mobile;
	//����
	tString mail;

	//ְλ
	tString post;

	// ���ѵ�����״̬
	int     status;
	 
	FriendListItemInfo* Clone();
	// 
	void SetTag(UINT_PTR tag);
	UINT_PTR GetTag();
private:
	UINT_PTR  tag;   
};

namespace DuiLib
{
class Node;
typedef std::map<tString,Node*>	Children;

struct NodeData
{
	int level_;
	bool folder_;
	bool child_visible_;
	bool has_child_;
	///*CDuiString*/  tString text_;
	//�����б���value��ʾ�����û���Id
	///*CDuiString*/  tString value; 
	CListContainerElementUI* list_elment_;
	FriendListItemInfo*    pItemData;
};

double CalculateDelay(double state);

class Node
{
public:
	Node();
	//Node(const Node& obj);
	explicit Node(NodeData t);
	Node(NodeData t, Node* parent);
	~Node();
	NodeData& data();
	int num_children() const;
	Node* child(int i);
	Node* parent();
	bool folder() const;
	bool has_children() const;
	void add_child(Node* child);
	void remove_child(Node* child);
	void remove_allchild();
	Node* get_last_child();
	Children& get_children();
	Children& get_children_();
	Node* Clone();
	Node*  GetlinkNode();
	void  SetlinkNode(Node* node);
private:
	void set_parent(Node* parent);

private:
//	typedef std::vector <Node*>	Children;
	Children	children_; // �����ߵĺ���
	Children	children ; // ���ߵģ������������߷�æ��
	Node*		parent_;

	NodeData    data_;
	Node*       linkNode_;
};

} // DuiLib

#endif // UILISTCOMMONDEFINE_HPP