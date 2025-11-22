# Security Policy

## Supported Versions

We release patches for security vulnerabilities. Which versions are eligible for receiving such patches depends on the CVSS v3.0 Rating:

| Version | Supported          |
| ------- | ------------------ |
| 0.1.x   | :white_check_mark: |

## Reporting a Vulnerability

Please report (suspected) security vulnerabilities to **[security@your-org.com](mailto:security@your-org.com)**. You will receive a response within 48 hours. If the issue is confirmed, we will release a patch as soon as possible depending on complexity but historically within a few days.

## Security Best Practices

When using this library:

1. **Never commit API keys** - Use environment variables or secure configuration management
2. **Use HTTPS** - All API calls use HTTPS by default
3. **Keep dependencies updated** - Regularly update Boost, simdjson, and OpenSSL
4. **Validate input** - Always validate user input before making API calls
5. **Handle errors securely** - Don't expose sensitive information in error messages

## Dependencies

We regularly update dependencies to address security vulnerabilities. See our [Dependabot configuration](.github/dependabot.yml) for automated dependency updates.

