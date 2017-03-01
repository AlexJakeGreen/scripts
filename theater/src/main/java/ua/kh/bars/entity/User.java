package ua.kh.bars.entity;

import java.util.Date;


public class User {
    private Long id;
    private String email;
    private String name;
    private int tickets;
    private Date birthday;
    private String systemMessage;

    public User() {
    }

    public User(String email, String name, Date birthday) {
        this.id = null;
        this.email = email;
        this.name = name;
        this.birthday = birthday;
        this.tickets = 0;
        this.systemMessage = "";
    }

    public User(Long id, String email, String name, Date birthday) {
        this(email, name, birthday);
        this.id = id;
    }

    public User(Long id, String email, String name, Date birthday, String systemMessage) {
        this(id, email, name, birthday);
        this.systemMessage = systemMessage;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public Long getId() {
        return id;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getEmail() {
        return email;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setTickets(int tickets) {
        this.tickets = tickets;
    }

    public int getTickets() {
        return tickets;
    }

    public void setBirthday(Date birthday) {
        this.birthday = birthday;
    }

    public Date getBirthday() {
        return this.birthday;
    }

    public boolean isLucky() {
        return Math.random() < 0.5;
    }

    public String getSystemMessage() {
        return systemMessage;
    }

    public void setSystemMessage(String message) {
        systemMessage = message;
    }
    
    public String toString() {
        return "User: {id: '" + id
            + "', email: '" + email
            + "', name: '" + name
            + "', birthday: '" + birthday
            + "', systemMessage: '" + systemMessage + "'}";
    }
}
