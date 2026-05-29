
### Step 1: Generate the SSH Key
Your friend needs to open their terminal (Git Bash on Windows, or the regular terminal on Mac/Linux) and run this command, replacing the email with their own GitHub email address:

```bash
ssh-keygen -t ed25519 -C "their-email@example.com"
```

Then, they should press **Enter** to accept the default file location. When asked for a passphrase, they can either create one for extra security or press Enter to skip it .

### Step 2: Add the Key to Their GitHub Account
1.  Run this command in the terminal to copy the **public key**:
    ```bash
    cat ~/.ssh/id_ed25519.pub
    ```
2.  Select and copy the entire output that starts with `ssh-ed25519`.
3.  Go to **GitHub.com** → Click their profile photo → **Settings** → **SSH and GPG keys** → **New SSH key**.
4.  Paste the key into the "Key" field, give it a descriptive title (e.g., "Personal Laptop"), and click **Add SSH key** .

### Step 3: Verify the Connection
Back in the terminal, your friend should run a test to confirm everything is working:

```bash
ssh -T git@github.com
```

If it's successful, they will see a message like:
> "Hi [their-username]! You've successfully authenticated, but GitHub does not provide shell access." 

Once they see that message, they can go back to their project folder and run `git clone` or `git push` again, and it should work perfectly .

---

### 💡 A Quick Alternative: Switching to HTTPS
If your friend finds the SSH setup too technical, or if they are behind a strict firewall, they can use the **HTTPS URL** instead. This bypasses the SSH key requirement entirely.

They can change the remote URL with this command:
```bash
git remote set-url origin https://github.com/your-username/your-repository.git
```
*Note: When pushing, they may need to enter a **Personal Access Token (PAT)** instead of their GitHub password. They can generate one in GitHub Settings under "Developer settings" > "Personal access tokens"* .