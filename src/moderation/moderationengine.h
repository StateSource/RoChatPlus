#ifndef MODERATIONENGINE_H
#define MODERATIONENGINE_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <memory>

class LinkValidator;

class ModerationEngine
{
public:
    ModerationEngine();
    ~ModerationEngine();

    bool validateLink(const QString &url);
    bool validateImage(const QByteArray &imageData);
    QString filterContent(const QString &content);
    QStringList extractLinks(const QString &text);
    
    void loadBlacklist(const QString &filePath);
    void updateBlacklist();
    
    bool isMaliciousLink(const QString &url) const;
    float getLinkTrustScore(const QString &url) const;

private:
    std::unique_ptr<LinkValidator> m_linkValidator;
    QStringList m_blacklist;
    QStringList m_whitelistDomains;
    
    void initializeBlacklist();
};

#endif // MODERATIONENGINE_H
