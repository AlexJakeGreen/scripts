package ua.kh.bars.entities;

import javax.persistence.*;

/**
 * Created by green on 4/3/16.
 */
@Entity
@Table(name = "user_accounts")
public class UserAccount {

    @Id
    @GeneratedValue(strategy= GenerationType.IDENTITY)
    private Long id;

    @OneToOne(optional = false, fetch = FetchType.EAGER)
    @JoinColumn(name = "user_id")
    private User user;

    @Column(name = "money_prepaid")
    private int moneyPrepaid;


    public UserAccount() {
    }

    public UserAccount(Long id, User user, int moneyPrepaid) {
        this.id = id;
        this.user = user;
        this.moneyPrepaid = moneyPrepaid;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public int getMoneyPrepaid() {
        return moneyPrepaid;
    }

    public void setMoneyPrepaid(int moneyPrepaid) {
        this.moneyPrepaid = moneyPrepaid;
    }

    @Override
    public String toString() {
        return "UserAccount{" +
                "id=" + id +
                ", user=" + user +
                ", moneyPrepaid=" + moneyPrepaid +
                '}';
    }
}
