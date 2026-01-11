#include "linkvalidator.h"
#include <QUrl>
#include <QRegularExpression>
#include <QDebug>

LinkValidator::LinkValidator()
{
    qDebug() << "LinkValidator initialized";
}

LinkValidator::~LinkValidator() = default;

bool LinkValidator::isValidUrl(const QString &urlString) const
{
    QUrl url(urlString);
    return url.isValid() && !url.scheme().isEmpty();
}

bool LinkValidator::isSafeUrl(const QString &urlString) const
{
    if (!isValidUrl(urlString)) {
        return false;
    }
    
    if (containsSuspiciousPatterns(urlString)) {
        return false;
    }
    
    if (isPhishingLike(urlString)) {
        return false;
    }
    
    QString domain = extractDomain(urlString);
    if (isDomainBlacklisted(domain)) {
        return false;
    }
    
    float reputationScore = calculateReputationScore(urlString);
    if (reputationScore < 0.8f) {
        return false;
    }
    
    return true;
}

bool LinkValidator::isDomainWhitelisted(const QString &domain) const
{
    const QStringList whitelist = {
        "roblox.com", "discord.com", "youtube.com", "twitch.tv", "github.com"
    };
    
    for (const QString &whitelistedDomain : whitelist) {
        if (domain.endsWith(whitelistedDomain, Qt::CaseInsensitive)) {
            return true;
        }
    }
    
    return false;
}

bool LinkValidator::isDomainBlacklisted(const QString &domain) const
{
    const QStringList blacklist = {
        "malicious.com", "phishing.net", "scam.org", "suspicious.net"
    };
    
    for (const QString &blacklistedDomain : blacklist) {
        if (domain.endsWith(blacklistedDomain, Qt::CaseInsensitive)) {
            return true;
        }
    }
    
    return false;
}

QString LinkValidator::extractDomain(const QString &urlString) const
{
    QUrl url(urlString);
    return url.host();
}

float LinkValidator::calculateReputationScore(const QString &urlString) const
{
    float score = 0.5f;  // Default neutral score
    
    QUrl url(urlString);
    QString domain = url.host();
    
    // Higher score for whitelisted domains
    if (isDomainWhitelisted(domain)) {
        return 1.0f;
    }
    
    // Lower score for blacklisted domains
    if (isDomainBlacklisted(domain)) {
        return 0.0f;
    }
    
    // Check for suspicious patterns
    if (containsSuspiciousPatterns(urlString)) {
        score -= 0.3f;
    }
    
    // Check for valid TLD
    if (hasValidTLD(domain)) {
        score += 0.2f;
    }
    
    // Check domain age (simulated)
    if (domain.length() > 10) {
        score += 0.1f;  // Longer, established domains are more trustworthy
    }
    
    // Clamp score between 0 and 1
    return qBound(0.0f, score, 1.0f);
}

bool LinkValidator::containsSuspiciousPatterns(const QString &urlString) const
{
    // Check for common phishing patterns
    QStringList suspiciousPatterns = {
        "bit.ly",      // URL shorteners can hide malicious URLs
        "tinyurl",
        "url.shortener",
        "-roblox",     // Homograph attacks (e.g., roblox-phishing.com)
    };
    
    for (const QString &pattern : suspiciousPatterns) {
        if (urlString.contains(pattern, Qt::CaseInsensitive)) {
            return true;
        }
    }
    
    // Check for IP addresses instead of domain names
    QRegularExpression ipRegex("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
    if (urlString.contains(ipRegex)) {
        return true;
    }
    
    return false;
}

bool LinkValidator::isPhishingLike(const QString &url) const
{
    // Check for common phishing indicators
    if (url.contains("roblox", Qt::CaseInsensitive) && 
        !url.contains("roblox.com", Qt::CaseInsensitive)) {
        // Contains "roblox" but not from official domain
        return true;
    }
    
    return false;
}

bool LinkValidator::hasValidTLD(const QString &domain) const
{
    const QStringList validTLDs = {
        ".com", ".org", ".net", ".edu", ".gov", ".io", ".co", ".tv", ".info", ".app", ".dev"
    };
    
    for (const QString &tld : validTLDs) {
        if (domain.endsWith(tld, Qt::CaseInsensitive)) {
            return true;
        }
    }
    
    return false;
}

bool LinkValidator::hasExcessiveSubdomains(const QString &domain) const
{
    int subdomainCount = domain.count('.');
    return subdomainCount > 3;  // More than 3 dots is suspicious
}
