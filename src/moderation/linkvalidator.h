#ifndef LINKVALIDATOR_H
#define LINKVALIDATOR_H

#include <QString>
#include <QUrl>

class LinkValidator
{
public:
    LinkValidator();
    ~LinkValidator();

    bool isValidUrl(const QString &urlString) const;
    bool isSafeUrl(const QString &urlString) const;
    bool isDomainWhitelisted(const QString &domain) const;
    bool isDomainBlacklisted(const QString &domain) const;
    
    QString extractDomain(const QString &urlString) const;
    float calculateReputationScore(const QString &urlString) const;
    
    bool containsSuspiciousPatterns(const QString &urlString) const;

private:
    bool isPhishingLike(const QString &url) const;
    bool hasValidTLD(const QString &domain) const;
    bool hasExcessiveSubdomains(const QString &domain) const;
};

#endif // LINKVALIDATOR_H
