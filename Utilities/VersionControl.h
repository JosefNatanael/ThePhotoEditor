#ifndef VERSIONCONTROL_H
#define VERSIONCONTROL_H

#include <QLinkedList>
#include <QImage>

class VersionControl
{
private:
    class MasterNode {
        QLinkedList<QImage>     sideBranch;
        QImage                  nodeImage;
        int                     sideBranchLength;

    public:
        MasterNode(QImage);

    public:
        int                     getBranchLength() const { return sideBranchLength; }
        void                    commitImage(QImage);
        void                    reverseCommit();
        const QImage&           getImageAtIndex(int index);
        bool                    canCommitImage() const { return sideBranchLength <= maxSideBranchLength - 1; }
        bool                    canReverseCommit() const { return sideBranchLength >= 2; }

    };

public:
    VersionControl(QImage);

    int                         getBranchLength() const { return masterBranchLength; }
    void                        commitImage(QImage);
    void                        reverseCommit();
    const QImage&               getImageAtIndex(int index);
    bool                        canReverseCommit() const { return masterBranchLength >= 2; }

private:
    QLinkedList<MasterNode>     masterBranch;
    int                         masterBranchLength = 0;
    static const int            maxMasterBranchLength = 5;
    static const int            maxSideBranchLength = 5;

};

#endif // VERSIONCONTROL_H
