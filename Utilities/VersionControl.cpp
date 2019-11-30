/**
 * @class VersionControl
 * @brief Version Control class, as a comprehensive image history/revision control.
 * @details A Version control is made up of a master branch, where each node of the branch is a node.
 * Each node in the master branch can also branch out to create a side branch.
 * The node of the master branch is called the masterNode,
 * while the node of the side branch is called the sideNode
 * Classes MasterNode and SideNode is the implementation of node in this version control.
 * A branch of the version control is a linked list with maximum length, maxMasterBranchLength and maxSideBranchLength.
 * When a branch has reached the maximum length it can store, 
 * additional commits to the branch will cause the oldest commit to be removed from the linked list.
 * MainWindow is a friend class of Version Control.
 */

#include "VersionControl.h"

/**
 * @brief Construct a new Version Control:: Version Control object
 * 
 * @param image Commits image to the version control object.
 * @param changes Commit message.
 */
VersionControl::VersionControl(QImage image, QString changes)
{
    masterBranch.push_front(MasterNode(image, changes));
    masterBranchLength = 1;
}

/**
 * @brief Construct a new Version Control:: Version Control object
 */
VersionControl::VersionControl()
{
    masterBranchLength = 0;
}

/**
 * @brief Construct a new Version Control:: Master Node:: Master Node object
 * 
 * @param image Commits image to the sideBranch of the master node.
 * @param changes Commit message.
 */
VersionControl::MasterNode::MasterNode(QImage image, QString changes)
    : changes(changes), sideBranchLength(1)
{
    sideBranch.push_front(SideNode(image, changes));
}

/**
 * @brief Commit changes to a sidebranch.
 * 
 * @param image Image to commit.
 * @param changes Commit message.
 */
void VersionControl::MasterNode::commitChanges(QImage image, QString changes)
{   
    // No need to pop back commit, if length is still below maximum length.
    if (sideBranchLength + 1 <= maxSideBranchLength) {
        sideBranch.push_front(SideNode(image, changes));
        ++sideBranchLength;
    }
    // Needs to remove the oldest commit.
    else {
        sideBranch.pop_back();
        sideBranch.push_front(SideNode(image, changes));
    }
}

/**
 * @brief Reverses a commit in a side branch.
 */
void VersionControl::MasterNode::reverseCommit()
{
    if (!canReverseCommit())
        return;
    sideBranch.pop_front();
    --sideBranchLength;
}

/**
 * @brief Gets const QImage& from index of a side branch.
 * 
 * @param index
 * @return const QImage& returned image at index.
 */
const QImage& VersionControl::MasterNode::getImageAtIndex(int index)
{
    QLinkedList<SideNode>::iterator it = sideBranch.begin();
    for (int i = 0; i < index - 1; ++i) {
        ++it;
    }
    return it->currentImage;
}

/**
 * @brief Commits changes.
 * 
 * @param image Image to commit.
 * @param changes Commit message.
 */
void VersionControl::commitChanges(QImage image, QString changes)
{
    // No need to pop back commit, if length is still below maximum length.
    if (masterBranchLength + 1 <= maxMasterBranchLength) {
        masterBranch.push_front(MasterNode(image, changes));
        ++masterBranchLength;
    }
    // Needs to remove the oldest commit.
    else {
        masterBranch.pop_back();
        masterBranch.push_front(MasterNode(image, changes));
    }
}

/**
 * @brief Reverse commit in the master branch.
 */
void VersionControl::reverseCommit()
{
    if (!canReverseCommit())
        return;
    masterBranch.pop_front();
    --masterBranchLength;
}

/**
 * @brief Gets const QImage& from index of master branch.
 * 
 * @param index 
 * @return const QImage& returned image at index.
 */
const QImage& VersionControl::getImageAtIndex(int index)
{
    return getMasterNodeIteratorAtIndex(index)->getImageAtIndex(0);
}

/**
 * @brief Gets a masternode from the master branch by index.
 * 
 * @param index 
 * @return QLinkedList<VersionControl::MasterNode>::iterator 
 */
QLinkedList<VersionControl::MasterNode>::iterator VersionControl::getMasterNodeIteratorAtIndex(int index)
{
    QLinkedList<MasterNode>::iterator it = masterBranch.begin();
    for (int i = 0; i < index; ++i) {
        ++it;
    }
    return it;
}
