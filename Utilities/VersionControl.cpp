#include "VersionControl.h"

VersionControl::VersionControl(QImage image, QString changes)
{
    masterBranch.push_front(MasterNode(image, changes));
    masterBranchLength = 1;
}

VersionControl::VersionControl()
{
    masterBranchLength = 0;
}

VersionControl::MasterNode::MasterNode(QImage image, QString changes)
    : changes(changes), sideBranchLength(1)
{
    sideBranch.push_front(SideNode(image, changes));
}

void VersionControl::MasterNode::commitChanges(QImage image, QString changes)
{
    if (!canCommitImage())
        return;
    sideBranch.push_front(SideNode(image, changes));
    ++sideBranchLength;
}

void VersionControl::MasterNode::reverseCommit()
{
    if (!canReverseCommit())
        return;
    sideBranch.pop_front();
    --sideBranchLength;
}

const QImage& VersionControl::MasterNode::getImageAtIndex(int index)
{
    QLinkedList<SideNode>::iterator it = sideBranch.begin();
    for (int i = 0; i < index - 1; ++i) {
        ++it;
    }
    return it->currentImage;
}

void VersionControl::commitChanges(QImage image, QString changes)
{
    if (masterBranchLength + 1 <= maxMasterBranchLength) {
        masterBranch.push_front(MasterNode(image, changes));
        ++masterBranchLength;
    }
    else {
        masterBranch.pop_back();
        masterBranch.push_front(MasterNode(image, changes));
    }
}

void VersionControl::reverseCommit()
{
    if (!canReverseCommit())
        return;
    masterBranch.pop_front();
    --masterBranchLength;
}

const QImage& VersionControl::getImageAtIndex(int index)
{
    QLinkedList<MasterNode>::iterator it = masterBranch.begin();
    for (int i = 0; i < index - 1; ++i) {
        ++it;
    }
    return it->getImageAtIndex(0);
}
