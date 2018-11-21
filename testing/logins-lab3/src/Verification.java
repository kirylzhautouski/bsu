import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Scanner;

public class Verification {

    public static final long MAXFILESIZE = 200000; // bytes

    private HashMap<String, String> logins;
    private int failCounter = 0;

    public Verification(String fileName) {
        File file = new File(fileName);

        if (!file.isFile())
            throw new IllegalArgumentException("file '" + fileName +  "' does not exist");

        if (file.length() > MAXFILESIZE)
            throw new IllegalArgumentException("file '" + fileName + "' is too large");

        if (file.length() == 0)
            throw new IllegalArgumentException("file '" + fileName + "' is empty");

        logins = new HashMap<>();
        try (FileInputStream inputStream = new FileInputStream(fileName);
             Scanner scanner = new Scanner(inputStream))
        {
            while (scanner.hasNextLine())
            {
                String[] userInfo = scanner.nextLine().split(" ");
                logins.put(userInfo[0], userInfo[1]);
            }
        }
        catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

    public VerificationInfo verificate(String login, String password) {
        if (failCounter == 3)
            return new VerificationInfo("no more attempts", false);

        if (login == null) {
            failCounter++;
            return new VerificationInfo("login is null", false);
        }

        if (password == null) {
            failCounter++;
            return new VerificationInfo("password is null", false);
        }

        if (login.length() == 0) {
            failCounter++;
            return new VerificationInfo("login is empty", false);
        }

        if (password.length() == 0) {
            failCounter++;
            return new VerificationInfo("password is empty", false);
        }

        if (login.length() > 10) {
            failCounter++;
            return new VerificationInfo("login is longer than 10", false);
        }

        if (password.length() > 8) {
            failCounter++;
            return new VerificationInfo("password is longer than 8", false);
        }

        try {
            if (logins.get(login).equals(password))
                return new VerificationInfo("Hi, " + login, true);
            else {
                failCounter++;
                return new VerificationInfo("invalid password", false);
            }
        } catch (NullPointerException ex) {

        }

        failCounter++;
        return new VerificationInfo("invalid login", false);
    }

}
